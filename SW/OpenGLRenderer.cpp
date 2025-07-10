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

    // --- ��ü �׸��� ---
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, atom.mol_size * 0.2f, 32, 32);  // ������, slice, stack
    gluDeleteQuadric(quad);

    // --- �ؽ�Ʈ ������ ---
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);  // ���� ����

    float zoomScale = 1.2f;
    float scaleFactor = std::clamp(1.0f / std::fabs(zoom * zoomScale), 0.4f, 5.0f);

    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0f);  // Ȯ�� ����

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

    // ��ġ ���� ����
    float dx = to->x - from->x;
    float dy = to->y - from->y;
    float dz = to->z - from->z;

    // ����
    float length = sqrtf(dx * dx + dy * dy + dz * dz);
    if (length < 1e-6f) return;

    // ���� ���� ����
    float ux = dx / length;
    float uy = dy / length;
    float uz = dz / length;

    // ȸ����: ������ z�� �������� �����, (dx,dy,dz) ������ ȸ��
    float angle = acosf(uz) * 180.0f / 3.14159f;
    float rx = -dy;
    float ry = dx;
    float rz = 0.0f;

    glPushMatrix();

    // ���������� �̵�
    glTranslatef(from->x, from->y, from->z);

    // ȸ�� (��, z��� ������ ������ ���� ��쿡�� ȸ���� �ʿ� ����)
    if (fabsf(uz) < 0.9999f)
        glRotatef(angle, rx, ry, rz);

    // ����
    glColor3f(0.99f, 0.99f, 0.99f);

    // ���� �׸���
    GLUquadric* quad = gluNewQuadric();
    float radius = 0.01f * bond.bond_order;  // ���� ���� ���� �β�
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

    // ī�޶� Z�� �ڷ� ���� (���� ��ü�� ���̵���)
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
    glColor3f(1.0f, 1.0f, 1.0f);  // ��� �ؽ�Ʈ
    glRasterPos3f(x, y, z);
    for (char c : text)
        glCallList(m_fontListBase + (unsigned char)c);
}