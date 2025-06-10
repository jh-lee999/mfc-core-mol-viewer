// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "ViewMenu1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(ViewMenu1, StyleDialog) 

ViewMenu1::ViewMenu1(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_MODALESS_MENU1, pParent) 
{
	m_pParent = pParent;
}

ViewMenu1::~ViewMenu1()
{
}

void ViewMenu1::UpdateLayerControl(int cx, int cy)
{
	
}

BOOL ViewMenu1::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetBackgroundColor(RGB(150, 150, 64));  // 배경 색상 
	
	StyleDialog::UpdateLayerControl(); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ViewMenu1::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	StyleDialog::DoDataExchange(pDX);

}
