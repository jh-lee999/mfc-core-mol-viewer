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
     Setzoom(-10.0f);
 
    // 위에서 정한 픽셀값을 정의함-> 이 시점부터 DC는 opengl dc가 됨.
    if (!SetPixelFormat(m_hDC, pixelFormat, &pfd)) return false;

    // HGLRC 생성, OpenGL의 상태값, 쉐이더, 버텍스 데이터 등 저장할 GL 컨텍스트의 공간
    m_hRC = wglCreateContext(m_hDC);
    if (!m_hRC) return false;
    // 만든 RC를 DC에 연결함

    if (!wglMakeCurrent(m_hDC, m_hRC)) return false;

    // ✅ 이 다음부터만 OpenGL 함수 호출이 유효함
    InitLighting();
    initFont();

    return true;
}

void OpenGL::initFont()
{
    HDC hdc = m_hDC;

    HFONT hFont = CreateFont(
        -14, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Consolas");

    SelectObject(hdc, hFont);

    m_fontListBase = glGenLists(256);
    wglUseFontBitmaps(hdc, 0, 255, m_fontListBase);
}


void OpenGL::InitLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);  // ← ✅ 꼭 있어야 하고

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // ← ✅ 이거 필수!!

    GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
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

    OpenGLRenderer renderer;
    float cx, cy, cz, size;
    renderer.GetModelCenter(cx, cy, cz, size);
    m_centerX = cx;
    m_centerY = cy;
    m_centerZ = cz;
    m_modelSize = size;

    // 뷰 세팅
    SetupViewTransform();
    // 화면 클리어
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawObject();
    DrawWorldAxisIndicator(width, height);

    

    

    SwapBuffers(m_hDC);
}


void OpenGL::SetRotation(float picth, float roll, float yaw)
{
    m_pitch = picth;
    m_yaw = yaw;
    m_roll = roll;
}


void OpenGL::DrawObject() 
{
    if (m_style == Viewer3D)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);   
    }
   OpenGLRenderer renderer;
   renderer.SetFontListBase(m_fontListBase);
   renderer.Draw(ObjectContainer::Get(),Getzoom());

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
        float pitchRad = m_pitch * 3.1415926f / 180.0f;
        float yawRad = m_yaw * 3.1415926f / 180.0f;

        float centerX = m_centerX;
        float centerY = m_centerY;
        float centerZ = m_centerZ;
        float modelSize = std::max(m_modelSize, 2.0f);

        float zoomBase = 1.2f;
        float zoomScale = powf(zoomBase, m_offsetZ);
        float distance = modelSize * 6.0f * zoomScale;

        float dirX = sinf(yawRad) * cosf(pitchRad);
        float dirY = sinf(pitchRad);
        float dirZ = cosf(yawRad) * cosf(pitchRad);

        float rightX = cosf(yawRad);
        float rightY = 0.0f;
        float rightZ = -sinf(yawRad);

        float upX = 0.0f;
        float upY = 1.0f;
        float upZ = 0.0f;

        float moveX = m_offsetX * rightX + m_offsetY * upX;
        float moveY = m_offsetX * rightY + m_offsetY * upY;
        float moveZ = m_offsetX * rightZ + m_offsetY * upZ;

        centerX += moveX;
        centerY += moveY;
        centerZ += moveZ;

        float camX = centerX + distance * dirX;
        float camY = centerY + distance * dirY;
        float camZ = centerZ + distance * dirZ;

        gluLookAt(camX, camY, camZ,
            centerX, centerY, centerZ,
            0.0f, 1.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        glTranslatef(m_offsetX, m_offsetY, 0.0f);
    }

    GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}




void OpenGL::DrawWorldAxisIndicator(int screenWidth, int screenHeight)
{
    const int viewportSize = 80;
    const int margin = 10;
    const int viewportX = screenWidth - viewportSize - margin;
    const int viewportY = margin;

    m_axisViewport = CRect(viewportX, viewportY, viewportX + viewportSize, viewportY + viewportSize);

    glViewport(viewportX, viewportY, viewportSize, viewportSize);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(m_pitch, 1, 0, 0);
    glRotatef(m_yaw, 0, 1, 0);
    glRotatef(m_roll, 0, 0, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.5f);

    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();

    glLineWidth(1.0f);

    // 🟡 중심 정육면체는 hover일 때만 그린다
    if (m_axisHovered)
    {
        glColor3f(1.0f, 1.0f, 0.2f); // 강조 색
        float s = 0.1f;

        glBegin(GL_QUADS);
        // 6면 그리기
        glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);
        glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s); glVertex3f(-s, s, s); glVertex3f(-s, -s, s);
        glVertex3f(-s, s, -s); glVertex3f(s, s, -s); glVertex3f(s, s, s); glVertex3f(-s, s, s);
        glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, -s, -s);
        glVertex3f(-s, -s, s); glVertex3f(-s, s, s); glVertex3f(s, s, s); glVertex3f(s, -s, s);
        glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, s, -s); glVertex3f(-s, s, -s);
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


void OpenGL::DrawSolidCube() {

    float s = 0.1f;  // 큐브 한 변의 절반

    glBegin(GL_QUADS);

    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);

    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, -s, s);

    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);

    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, -s, -s);

    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);

    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(-s, s, -s);

    glEnd();


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

void OpenGL::ScreenToGL(int screenX, int screenY, float& glX, float& glY)
{
    RECT rect;
    ::GetClientRect(m_hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)screenX;
    winY = (float)(viewport[3] - screenY);

    glReadPixels(screenX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);  

    if (gluUnProject(winX, winY, 0.5f, modelview, projection, viewport, &posX, &posY, &posZ))
    {
        glX = static_cast<float>(posX);
        glY = static_cast<float>(posY);
    }
    else
    {
        glX = glY = 0.0f;
    }
}

bool OpenGL::ScreenToGL_3D(int screenX, int screenY, float& outX, float& outY, float& outZ)
{
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    // 화면 기준 마우스 좌표
    GLfloat winX = static_cast<GLfloat>(screenX);
    GLfloat winY = static_cast<GLfloat>(viewport[3] - screenY);  // Y 반전

    // 시작점 (Near)
    GLdouble startX, startY, startZ;
    if (!gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &startX, &startY, &startZ)) return false;

    // 끝점 (Far)
    GLdouble endX, endY, endZ;
    if (!gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &endX, &endY, &endZ)) return false;

    // 원하는 Z 위치 (예: 고정된 월드 좌표 z = -5.0f 기준 평면)
    float targetZ = -5.0f;

    // 방향 벡터
    float dirX = static_cast<float>(endX - startX);
    float dirY = static_cast<float>(endY - startY);
    float dirZ = static_cast<float>(endZ - startZ);

    if (fabs(dirZ) < 1e-6f) return false;  // z축 평행이면 무시

    // t: start → end 사이에서 z = targetZ에 도달하는 비율
    float t = (targetZ - static_cast<float>(startZ)) / dirZ;

    outX = static_cast<float>(startX) + dirX * t;
    outY = static_cast<float>(startY) + dirY * t;
    outZ = targetZ;

    return true;
}



void OpenGL::HandleMouseMove(int mouseX, int mouseY)
{
    // Y축 반전 필요할 수 있음 (MFC는 안 해도 됨)
    m_axisHovered = m_axisViewport.PtInRect(CPoint(mouseX, mouseY));
}

void OpenGL::HandleMouseClick(int mouseX, int mouseY)
{
    if (m_axisHovered)
    {
        int cx = m_axisViewport.left + m_axisViewport.Width() / 2;
        int cy = m_axisViewport.top + m_axisViewport.Height() / 2;

    }
}

void OpenGL::ResetViewPoint() {

    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    float m_roll = 0.0f;

    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;
    float m_offsetZ = 0.0f;


}