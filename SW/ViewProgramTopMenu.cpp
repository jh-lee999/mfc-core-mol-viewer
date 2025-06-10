// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "ViewProgramTopMenu.h"

#include "DlgInquiryLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(ViewProgramTopMenu, StyleDialog) 

ViewProgramTopMenu::ViewProgramTopMenu(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_VIEW_PROGRAM_TOP_MENU, pParent)
{
	m_pParent = pParent;
}

ViewProgramTopMenu::~ViewProgramTopMenu()
{
}

void ViewProgramTopMenu::UpdateLayerControl(int cx, int cy)
{

}

BOOL ViewProgramTopMenu::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetBackgroundColor(RGB(230, 230, 230));  // 배경 색상 
	
	COLORREF activateColor = RGB(64, 64, 64); // 버튼 색상 지정

	

	
	StyleDialog::UpdateLayerControl(); // 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ViewProgramTopMenu::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	StyleDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(ViewProgramTopMenu, StyleDialog)
END_MESSAGE_MAP()




