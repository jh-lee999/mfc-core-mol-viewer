#pragma once

#include "pch.h"
#include <gl/GL.h>
#include <gl/GLU.h>


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
	void Cleanuo();
	void Setoffset(float x, float y);
	void Setzoom(float z);
	float Getzoom();
	void Render();
	void SetRotation(float picth, float roll, float yaw);

	void DrawObject();

	void SetupProjection(int width, int height);
	void SetupViewTransform();

	void d_ChangeViewer(ViewerStyle Style);

	const ViewerStyle GetCurrentStyle() { return m_style; };

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

};

