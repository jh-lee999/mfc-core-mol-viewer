#include "pch.h"
#include "OpenGL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

OpenGL::OpenGL()
{
}

OpenGL::~OpenGL()
{
}

bool OpenGL::init(HWND hWnd, ViewerStyle Style)
{
    m_hWnd = hWnd;
    m_hDC = ::GetDC(hWnd);
    
    // DC�� � ����� �ȼ� �����͸� ����� ������ ���� �����ϴ� ����ü
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //�����쿡 ���� �׸� �� ����, GL ����, ���� ���۸� ����
        PFD_TYPE_RGBA, // ���� ����
        32, //32��Ʈ �÷� 
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // ���� ���� �뷮
        8, // ���ٽ� ���� �뷮
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    // ���� pfd���� ���� ���� ����� �ȼ� ������ ���ش޶�� ��û
    int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    if (pixelFormat == 0) return false;

    if (m_style != Viewer2D && m_style != Viewer3D) return false;
     m_style = Style;
     Setzoom(-5.0f);
    // ������ ���� �ȼ����� ������-> �� �������� DC�� opengl dc�� ��.
    if (!SetPixelFormat(m_hDC, pixelFormat, &pfd)) return false;

    // HGLRC ����, OpenGL�� ���°�, ���̴�, ���ؽ� ������ �� ������ GL ���ؽ�Ʈ�� ����
    m_hRC = wglCreateContext(m_hDC);
    if (!m_hRC) return false;
    // ���� RC�� DC�� ������
    return wglMakeCurrent(m_hDC, m_hRC) == TRUE;

}

void OpenGL::Cleanuo()
{
    if (m_hRC)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hRC);
        m_hRC = nullptr;
    }

    if (m_hDC)
    {
        // HWND�� ViewMain���� �����ϹǷ� ReleaseDC��
        // ::ReleaseDC(hWnd, m_hDC); �� hWnd ���� ���� �������� �ܺο��� �ؾ� ��
        m_hDC = nullptr;
    }
}

void OpenGL::Setoffset(float x, float y)
{
    m_offsetX = x;
    m_offsetY = y;
}

void OpenGL::Setzoom(float z)
{
    m_offsetZ = z;
}
float OpenGL::Getzoom()
{
    return m_offsetZ;
}

void OpenGL::Render()
{
    if (!m_hDC || !m_hRC) return;

    wglMakeCurrent(m_hDC, m_hRC);

    // ���� ���� ũ�� Ȯ��
    CRect rect;
    ::GetClientRect(m_hWnd, &rect);
    int width = rect.Width();
    int height = rect.Height();
    if (height == 0) height = 1;



    SetupProjection(width, height);

    // �� ������ ����Ʈ ��Ȯ��
    glViewport(0, 0, width, height);

    // �� ����
    SetupViewTransform();
    // ȭ�� Ŭ����
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawObject();


    SwapBuffers(m_hDC);
}


void OpenGL::SetRotation(float picth, float roll, float yaw)
{
    m_pitch = picth;
    m_yaw = yaw;
    m_roll = roll;
}


void OpenGL::DrawObject() {


    if (m_style == Viewer3D)
    {
        // ���� 3D �ﰢ��
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0, 1, 0); glVertex3f(-1.0f, -1.0f, 0.5f);
        glColor3f(0, 0, 1); glVertex3f(1.0f, -1.0f, -0.5f);
        glEnd();
    }
    else if (m_style == Viewer2D)
    {
        // 2D ���� �ﰢ��: ������ ũ�� ���� ��ǥ
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2f(100.0f, 100.0f);
        glColor3f(0, 1, 0); glVertex2f(200.0f, 100.0f);
        glColor3f(0, 0, 1); glVertex2f(150.0f, 200.0f);
        glEnd();
    }

}

void OpenGL::SetupProjection(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (m_style == Viewer3D)
    {
        gluPerspective(45.0, (GLfloat)width / height, 1.0, 100.0);
    }
    else
    {
        float zoomBase = 1.2f;
        float zoomScale = powf(zoomBase, m_offsetZ);
        if (zoomScale < 0.05f) zoomScale = 0.05f;
        if (zoomScale > 20.0f) zoomScale = 20.0f;

        float halfWidth = (width / 2.0f) * zoomScale;
        float halfHeight = (height / 2.0f) * zoomScale;

        glOrtho(
            -halfWidth, halfWidth,
            -halfHeight, halfHeight,
            -1.0, 1.0
        );
    }
}



void OpenGL::SetupViewTransform() 
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_style == Viewer3D)
    {
        glTranslatef(m_offsetX, m_offsetY, m_offsetZ);
        glRotatef(m_pitch, 1, 0, 0);
        glRotatef(m_yaw, 0, 1, 0);
        glRotatef(m_roll, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        glTranslatef(m_offsetX, m_offsetY, 0.0f);
    }

}

void OpenGL::d_ChangeViewer(ViewerStyle Style)
{
    m_style = Style;

    // ���� �ʱ�ȭ
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_offsetZ = 0.0f;
    m_pitch = 0.0f;
    m_yaw = 0.0f;
    m_roll = 0.0f;

    // ��Ÿ�Ϻ� �⺻ �� �Ÿ� ����
    if (m_style == Viewer3D)
    {
        m_offsetZ = -5.0f;  // 3D ī�޶� �⺻ �Ÿ�
    }
    else if (m_style == Viewer2D)
    {
        m_offsetZ = 0.0f;   // 2D������ Zoom ������ ���
    }

    // ���� ���� + ������
    CRect rect;
    ::GetClientRect(m_hWnd, &rect);
    SetupProjection(rect.Width(), rect.Height());
    Render();
}

