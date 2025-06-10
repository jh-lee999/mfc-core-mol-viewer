// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "ViewMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(ViewMain, StyleDialog) 

ViewMain::ViewMain(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_VIEW_MAIN, pParent)
{
	m_pParent = pParent;
}

ViewMain::~ViewMain()
{
}

void ViewMain::UpdateLayerControl(int cx, int cy)
{
	m_pictureCtl.MoveWindow(5, 5, cy  - 10, cy  - 10);

}

BOOL ViewMain::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetBackgroundColor(RGB(64, 64, 64));  // 배경 색상 

	HWND hWndGL = m_pictureCtl.GetSafeHwnd();
	m_gl.init(hWndGL, Viewer3D);
	m_gl.Render();
	SetTimer(1, 16, nullptr);
	StyleDialog::UpdateLayerControl(); // 리소스 배치 함수	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ViewMain::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	StyleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPENCV, m_pictureCtl);
}
BEGIN_MESSAGE_MAP(ViewMain, StyleDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &ViewMain::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


void ViewMain::OnBnClickedButton1()
{
	static bool isClicked = false;
	isClicked = !isClicked;
	m_mouseCtrl.ResetViewPoint();
	if (isClicked == true)
	{
		m_gl.d_ChangeViewer(Viewer2D);
	}
	else 
	{
		m_gl.d_ChangeViewer(Viewer3D);
	}
}


void ViewMain::OnTimer(UINT_PTR nIDEvent)
{

	m_gl.Render();
	StyleDialog::OnTimer(nIDEvent);
}


void ViewMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_mouseCtrl.StartDrag(point);
	SetCapture();
	StyleDialog::OnLButtonDown(nFlags, point);
}


void ViewMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_mouseCtrl.EndDrag();
	ReleaseCapture();
	StyleDialog::OnLButtonUp(nFlags, point);
}


void ViewMain::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_mouseCtrl.EndDrag();
	ReleaseCapture();
	StyleDialog::OnRButtonUp(nFlags, point);
}


void ViewMain::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_mouseCtrl.StartDrag(point);
	SetCapture();
	StyleDialog::OnRButtonDown(nFlags, point);
}


void ViewMain::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mouseCtrl.UpdateDrag(nFlags, point, m_gl.GetCurrentStyle(), m_gl);

	StyleDialog::OnMouseMove(nFlags, point);
}




BOOL ViewMain::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 마우스 커서 위치 얻기
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	// OpenGL Static 컨트롤의 위치 얻기
	CRect rect;
	GetDlgItem(IDC_STATIC_OPENCV)->GetWindowRect(&rect);

	// 커서가 OpenGL 컨트롤 위에 있을 때만 처리
	if (rect.PtInRect(cursorPos))
	{
		float zoom = (zDelta > 0) ? 0.5f : -0.5f;
		m_gl.Setzoom(m_gl.Getzoom() + zoom);  // GetZoom()은 m_offsetZ 리턴
		m_gl.Render();
	}

	return StyleDialog::OnMouseWheel(nFlags, zDelta, pt);
}

