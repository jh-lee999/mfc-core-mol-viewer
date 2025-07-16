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

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, atom.mol_size * 0.2f, 32, 32);
    gluDeleteQuadric(quad);

    glPopMatrix();
}



void OpenGLRenderer::DrawBond(const Bond& bond, float zoom)
{
    const Atom* from = ObjectContainer::Get().GetAtomObject(bond.from_id);
    const Atom* to = ObjectContainer::Get().GetAtomObject(bond.to_id);
    if (!from || !to) return;

    float dx = to->x - from->x;
    float dy = to->y - from->y;
    float dz = to->z - from->z;

    float length = sqrtf(dx * dx + dy * dy + dz * dz);
    if (length < 1e-6f) return;

    float ux = dx / length;
    float uy = dy / length;
    float uz = dz / length;

    // 결합 벡터에 수직인 방향 벡터 계산
    float nx = -uy;
    float ny = ux;
    float nz = 0.0f;
    float nLength = sqrtf(nx * nx + ny * ny + nz * nz);
    if (nLength < 1e-6f) {
        // 결합이 XY 평면에 수직이면 Z축으로 오프셋 벡터 설정
        nx = 0;
        ny = -uz;
        nz = uy;
        nLength = sqrtf(nx * nx + ny * ny + nz * nz);
    }
    nx /= nLength;
    ny /= nLength;
    nz /= nLength;

    // 기본 오프셋 간격
    float offsetScale = 0.2f;
    int bondCount = bond.bond_order;

    for (int i = 0; i < bondCount; ++i)
    {
        // 가운데부터 좌우로 균등하게 배치: (-1, 0, 1) 등
        float offset = (i - (bondCount - 1) / 2.0f) * offsetScale;

        float fx = from->x + offset * nx;
        float fy = from->y + offset * ny;
        float fz = from->z + offset * nz;

        glPushMatrix();
        glTranslatef(fx, fy, fz);

        float angle = acosf(uz) * 180.0f / 3.14159f;
        float rx = -dy;
        float ry = dx;
        float rz = 0.0f;
        if (fabsf(uz) < 0.9999f)
            glRotatef(angle, rx, ry, rz);

        glColor3f(0.99f, 0.99f, 0.99f);
        GLUquadric* quad = gluNewQuadric();
        float radius = 0.03f;
        gluCylinder(quad, radius, radius, length, 12, 1);
        gluDeleteQuadric(quad);

        glPopMatrix();
    }
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

void OpenGLRenderer::GetModelCenter(float& outX, float& outY, float& outZ, float& outSize)
{
    BoundingBox box = ComputeBoundingBox();
    outX = (box.minX + box.maxX) / 2.0f;
    outY = (box.minY + box.maxY) / 2.0f;
    outZ = (box.minZ + box.maxZ) / 2.0f;

    float sizeX = box.maxX - box.minX;
    float sizeY = box.maxY - box.minY;
    float sizeZ = box.maxZ - box.minZ;
    outSize = std::max({ sizeX, sizeY, sizeZ });
}