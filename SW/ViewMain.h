#pragma once
#include "../CustomControl/include/StyleDialog.h"
#include "../CustomControl/include/StyleButton.h"
#include "../CustomControl/include/StyleStatic.h"
#include "../CustomControl/include/StyleStaticCheckBox.h"
#include <vector>
#include "OpenGLMouseController.h"
#include "OpenGL.h"

class ViewMain : public StyleDialog
{
	DECLARE_DYNAMIC(ViewMain)

public:
	ViewMain(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ViewMain();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_MAIN}; 
#endif

protected:
	CFont m_fontRecipeName;
	virtual void DoDataExchange(CDataExchange* pDX);
	void UpdateLayerControl(int cx, int cy) override;

private:
	CWnd* m_pParent;     // 부모 윈도우의 포인터
public:

	virtual BOOL OnInitDialog();

	CStatic m_pictureCtl;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	OpenGL m_gl;
	OpenGLMouseController m_mouseCtrl;

	BOOL m_isDragging = FALSE;
	CPoint m_lastMouse;

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_roll = 0.0f;
	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
