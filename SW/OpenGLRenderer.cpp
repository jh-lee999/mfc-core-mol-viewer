#include "pch.h"
#include "OpenGLRenderer.h"

#include <cmath>
#include "Colors.h"
#include "OpenGL.h"
#include <algorithm>

constexpr float ATOM_RADIUS = 0.1f;

void OpenGLRenderer::DrawAtom(const Atom& atom, float zoom)
{
    const ColorRGB color = GetColor(atom.mol_color);
    glColor3f(color.r, color.g, color.b);

    glPushMatrix();
    glTranslatef(atom.x, atom.y, atom.z);

    // --- 구체 그리기 ---
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, atom.mol_size * 0.2f, 32, 32);  // 반지름, slice, stack
    gluDeleteQuadric(quad);

    // --- 텍스트 렌더링 ---
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);  // 검정 글자

    float zoomScale = 1.2f;
    float scaleFactor = std::clamp(1.0f / std::fabs(zoom * zoomScale), 0.4f, 5.0f);

    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0f);  // 확대 먼저

    glRasterPos3f(0.0f, 0.0f, atom.mol_size * 0.21f);

    const char* label = atom.name.c_str();
    for (const char* c = label; *c; ++c)
        glCallList(m_fontListBase + (unsigned char)*c);

    glPopMatrix(); 

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();  
}



void OpenGLRenderer::DrawBond(const Bond& bond, float zoom)
{
    const Atom* from = ObjectContainer::Get().GetAtomObject(bond.from_id);
    const Atom* to = ObjectContainer::Get().GetAtomObject(bond.to_id);
    if (!from || !to) return;

    // 위치 벡터 차이
    float dx = to->x - from->x;
    float dy = to->y - from->y;
    float dz = to->z - from->z;

    // 길이
    float length = sqrtf(dx * dx + dy * dy + dz * dz);
    if (length < 1e-6f) return;

    // 단위 방향 벡터
    float ux = dx / length;
    float uy = dy / length;
    float uz = dz / length;

    // 회전축: 원통을 z축 기준으로 만들고, (dx,dy,dz) 쪽으로 회전
    float angle = acosf(uz) * 180.0f / 3.14159f;
    float rx = -dy;
    float ry = dx;
    float rz = 0.0f;

    glPushMatrix();

    // 시작점으로 이동
    glTranslatef(from->x, from->y, from->z);

    // 회전 (단, z축과 방향이 완전히 같을 경우에는 회전할 필요 없음)
    if (fabsf(uz) < 0.9999f)
        glRotatef(angle, rx, ry, rz);

    // 색상
    glColor3f(0.99f, 0.99f, 0.99f);

    // 원통 그리기
    GLUquadric* quad = gluNewQuadric();
    float radius = 0.01f * bond.bond_order;  // 결합 차수 따라 두께
    gluCylinder(quad, radius, radius, length, 12, 1);
    gluDeleteQuadric(quad);

    glPopMatrix();
}

void OpenGLRenderer::CenterView()
{
    BoundingBox box = ComputeBoundingBox();

    float centerX = (box.minX + box.maxX) / 2.0f;
    float centerY = (box.minY + box.maxY) / 2.0f;
    float centerZ = (box.minZ + box.maxZ) / 2.0f;

    float sizeX = box.maxX - box.minX;
    float sizeY = box.maxY - box.minY;
    float sizeZ = box.maxZ - box.minZ;
    float maxSize = std::max({ sizeX, sizeY, sizeZ });

    // 카메라를 Z축 뒤로 뺀다 (원자 전체가 보이도록)
    float cameraDistance = maxSize * 2.5f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        centerX, centerY, centerZ + cameraDistance,  // eye
        centerX, centerY, centerZ,                  // center
        0.0f, 1.0f, 0.0f                            // up
    );
}


void OpenGLRenderer::DrawText3D(const std::string& text, float x, float y, float z)
{
    glColor3f(1.0f, 1.0f, 1.0f);  // 흰색 텍스트
    glRasterPos3f(x, y, z);
    for (char c : text)
        glCallList(m_fontListBase + (unsigned char)c);
}