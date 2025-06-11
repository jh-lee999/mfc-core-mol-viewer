#include "pch.h"
#include "OpenGLMouseController.h"

void OpenGLMouseController::StartDrag(CPoint pt)
{
    m_isDragging = TRUE;
    m_lastMouse = pt;
}

void OpenGLMouseController::EndDrag()
{
    m_isDragging = FALSE;
}

void OpenGLMouseController::UpdateDrag(UINT nFlags, CPoint pt, ViewerStyle style, OpenGL& gl)
{
    if (!m_isDragging) return;

    float dx = static_cast<float>(pt.x - m_lastMouse.x);
    float dy = static_cast<float>(pt.y - m_lastMouse.y);

    float zoom = gl.Getzoom();  
    float scale = (style == Viewer3D) ? fabs(zoom) * 0.002f : 1.0f;
    float rotateScale = max(0.5f, fabs(zoom) * 0.02f);
    if (nFlags & MK_LBUTTON)
    {
        if (style == Viewer3D)
        {
            m_yaw += -dx * rotateScale;     // 수평 회전은 유지
            m_pitch += +dy * rotateScale;
        }
    }
    else if (nFlags & MK_RBUTTON)
    {
        m_offsetX += dx * scale;
        m_offsetY -= dy * scale;
    }

    m_lastMouse = pt;

    gl.Setoffset(m_offsetX, m_offsetY);
    gl.SetRotation(m_pitch, m_yaw, m_roll);
    gl.Render();
}

void OpenGLMouseController::ResetViewPoint() 
{
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_pitch = 0.0f;
    m_yaw = 0.0f;
    m_roll = 0.0f;

}