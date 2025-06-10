#pragma once

#include "OpenGL.h"

class OpenGLMouseController
{
public:
    void StartDrag(CPoint pt);
    void UpdateDrag(UINT nFlags, CPoint pt, ViewerStyle style, OpenGL& gl);
    void ResetViewPoint();
    void EndDrag();

private:
    BOOL m_isDragging = FALSE;
    CPoint m_lastMouse;
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;
    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    float m_roll = 0.0f;
};
