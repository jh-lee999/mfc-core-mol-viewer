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
    
    // DC에 어떤 방식의 픽셀 데이터를 사용할 건지에 대해 정의하는 구조체
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //윈도우에 직접 그릴 수 있음, GL 지원, 더블 버퍼링 지원
        PFD_TYPE_RGBA, // 색상 포멧
        32, //32비트 컬러 
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // 깊이 버퍼 용량
        8, // 스텐실 버퍼 용량
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    // 위의 pfd에서 정한 가장 비슷한 픽셀 포멧을 정해달라고 요청
    int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    if (pixelFormat == 0) return false;

    if (m_style != Viewer2D && m_style != Viewer3D) return false;
     m_style = Style;
     Setzoom(-5.0f);
    // 위에서 정한 픽셀값을 정의함-> 이 시점부터 DC는 opengl dc가 됨.
    if (!SetPixelFormat(m_hDC, pixelFormat, &pfd)) return false;

    // HGLRC 생성, OpenGL의 상태값, 쉐이더, 버텍스 데이터 등 저장할 GL 컨텍스트의 공간
    m_hRC = wglCreateContext(m_hDC);
    if (!m_hRC) return false;
    // 만든 RC를 DC에 연결함
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
        // HWND는 ViewMain에서 관리하므로 ReleaseDC만
        // ::ReleaseDC(hWnd, m_hDC); ← hWnd 따로 관리 안했으니 외부에서 해야 함
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

    // 현재 영역 크기 확인
    CRect rect;
    ::GetClientRect(m_hWnd, &rect);
    int width = rect.Width();
    int height = rect.Height();
    if (height == 0) height = 1;



    SetupProjection(width, height);

    // 매 프레임 뷰포트 재확인
    glViewport(0, 0, width, height);

    // 뷰 세팅
    SetupViewTransform();
    // 화면 클리어
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
        // 기존 3D 삼각형
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0, 1, 0); glVertex3f(-1.0f, -1.0f, 0.5f);
        glColor3f(0, 0, 1); glVertex3f(1.0f, -1.0f, -0.5f);
        glEnd();
    }
    else if (m_style == Viewer2D)
    {
        // 2D 기준 삼각형: 윈도우 크기 기준 좌표
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

    // 공통 초기화
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_offsetZ = 0.0f;
    m_pitch = 0.0f;
    m_yaw = 0.0f;
    m_roll = 0.0f;

    // 스타일별 기본 줌 거리 설정
    if (m_style == Viewer3D)
    {
        m_offsetZ = -5.0f;  // 3D 카메라 기본 거리
    }
    else if (m_style == Viewer2D)
    {
        m_offsetZ = 0.0f;   // 2D에서는 Zoom 비율로 사용
    }

    // 투영 갱신 + 렌더링
    CRect rect;
    ::GetClientRect(m_hWnd, &rect);
    SetupProjection(rect.Width(), rect.Height());
    Render();
}

