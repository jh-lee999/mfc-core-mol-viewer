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

    if (!from || !to)
        return;

    glColor3f(0.8f, 0.8f, 0.8f); // ������ ȸ��

    glLineWidth(static_cast<GLfloat>(bond.bond_order));  // ���� ������ ���� �� ����

    glBegin(GL_LINES);
    glVertex3f(from->x, from->y, from->z);
    glVertex3f(to->x, to->y, to->z);
    glEnd();
}

void OpenGLRenderer::DrawText3D(const std::string& text, float x, float y, float z)
{
    glColor3f(1.0f, 1.0f, 1.0f);  // ��� �ؽ�Ʈ
    glRasterPos3f(x, y, z);
    for (char c : text)
        glCallList(m_fontListBase + (unsigned char)c);
}