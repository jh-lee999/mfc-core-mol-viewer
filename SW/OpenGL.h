#pragma once

#include "pch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>
#include "OpenGLRenderer.h"

enum ViewerStyle
{
	Viewer3D,
	Viewer2D
};


class OpenGL
{
public:
	OpenGL();
	~OpenGL();

	bool init(HWND hWnd, ViewerStyle Style);
	void initFont();
	void InitLighting();
	void Cleanuo();
	void Setoffset(float x, float y);
	void Setzoom(float z);
	float Getzoom();
	void Render();
	void SetRotation(float picth, float roll, float yaw);
	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }
	
	void DrawObject();

	void SetupProjection(int width, int height);
	void SetupViewTransform();

	void DrawWorldAxisIndicator(int screenWidth, int screenHeight);

	void DrawSolidCube();

	void d_ChangeViewer(ViewerStyle Style);

	void ScreenToGL(int screenX, int screenY, float& glX, float& glY);

	bool ScreenToGL_3D(int screenX, int screenY, float& outX, float& outY, float& outZ);

	void HandleMouseMove(int mouseX, int mouseY);

	void HandleMouseClick(int mouseX, int mouseY);

	void ResetViewPoint();

	const ViewerStyle GetCurrentStyle() { return m_style; }

	void SetAddMode(bool enable) { m_isAddAtomMode = enable;    std::cout << "Add 모드 활성화" << "\n";
	}
	bool GetAddMode() {return m_isAddAtomMode;}
private:
	ViewerStyle m_style = Viewer3D;
	HDC m_hDC = nullptr;
	HGLRC m_hRC = nullptr;
	HWND m_hWnd = nullptr;
	
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_roll = 0.0f;

	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;
	float m_offsetZ = 0.0f;
	bool m_isAddAtomMode = false; 
	GLuint m_fontListBase = 0;
	CRect m_axisViewport;     
	bool m_axisHovered = false;


	float m_centerX ;
	float m_centerY ;
	float m_centerZ ;
	float m_modelSize ;
};

