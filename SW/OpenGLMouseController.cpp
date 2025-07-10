#include "pch.h"
#include "OpenGLMouseController.h"
#include <algorithm>

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
    float rotateScale = std::max(0.5f, fabs(zoom) * 0.02f);

    if (nFlags & MK_LBUTTON)
    {
        if (style == Viewer3D)
        {
            this->m_yaw -= dx * rotateScale;
            m_pitch += dy * rotateScale;

            m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

            // ⬇️ 여기 추가
            if (m_yaw > 180.0f) m_yaw -= 360.0f;
            else if (m_yaw < -180.0f) m_yaw += 360.0f;
        }
    }
    else if ((nFlags & MK_RBUTTON) || (nFlags & MK_MBUTTON))
    {
        m_offsetX -= dx * scale;
        m_offsetY += dy * scale;  
    }

    m_lastMouse = pt;

    gl.Setoffset(m_offsetX, m_offsetY);
    gl.SetRotation(m_pitch, m_roll, this->m_yaw);
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





/// 해야할 사항

// offset 이동 관련해서, orbit 관련 적용을 시켰는데, 그 이후 회전 시킨 후에 이상해짐.