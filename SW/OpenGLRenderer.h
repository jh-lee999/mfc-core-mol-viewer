#pragma once
#include "ObjectContainer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>

#undef min
#undef max



class OpenGLRenderer
{
public:

    struct BoundingBox {
        float minX, minY, minZ;
        float maxX, maxY, maxZ;
    };

    BoundingBox ComputeBoundingBox() {
        BoundingBox box = { FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (const auto& atom : ObjectContainer::Get().GetAllMolecules()) {
            box.minX = std::min(box.minX, atom.x);
            box.minY = std::min(box.minY, atom.y);
            box.minZ = std::min(box.minZ, atom.z);
            box.maxX = std::max(box.maxX, atom.x);
            box.maxY = std::max(box.maxY, atom.y);
            box.maxZ = std::max(box.maxZ, atom.z);
        }

        return box;
    }

    void Draw(const ObjectContainer& obj, float zoom)
    {
        for (const auto& atom : obj.GetAllMolecules())
            DrawAtom(atom,zoom);

        for (const auto& bond : obj.GetAllBonds())
            DrawBond(bond, zoom);
    }
    void SetFontListBase(GLuint base) { m_fontListBase = base; }
    void CenterView();
    void GetModelCenter(float& outX, float& outY, float& outZ, float& outSize);

private:
    void DrawAtom(const Atom& atom, float zoom);
    void DrawBond(const Bond& bond, float zoom);
    GLuint m_fontListBase = 0;
};