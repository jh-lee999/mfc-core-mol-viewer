#pragma once
#include "ObjectContainer.h"
#include <GL/gl.h>
#include <GL/glu.h>

class OpenGLRenderer
{
public:
    void Draw(const ObjectContainer& obj, float zoom)
    {
        for (const auto& atom : obj.GetAllMolecules())
            DrawAtom(atom,zoom);

        for (const auto& bond : obj.GetAllBonds())
            DrawBond(bond, zoom);
    }
    void SetFontListBase(GLuint base) { m_fontListBase = base; }

private:
    void DrawAtom(const Atom& atom, float zoom);
    void DrawBond(const Bond& bond, float zoom);

    void DrawText3D(const std::string& text, float x, float y, float z);
    GLuint m_fontListBase = 0;
};