// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "ViewProgramTopMenu.h"
#include "DlgInquiryLog.h"
#include "OpenGL.h"
#include "ViewMain.h"

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
	SetBackgroundColor(RGB(240, 240, 240));  // 배경 색상 
	
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
	ON_BN_CLICKED(IDC_ADDATOM, &ViewProgramTopMenu::OnBnClickedAddatom)
	ON_BN_CLICKED(IDC_BTN_DATA_TEST, &ViewProgramTopMenu::OnBnClickedBtnDataTest)
END_MESSAGE_MAP()






void ViewProgramTopMenu::OnBnClickedAddatom()
{
	BOOL isChecked = (IsDlgButtonChecked(IDC_ADDATOM) == BST_CHECKED);
	HWND hMainWnd = g_program->GetMVInstance()->GetSafeHwnd();
	if (isChecked)
	{
		::PostMessage(hMainWnd, WM_USER_ADD_ATOM_MODE, 1, 0);  // 1은 활성화
	}
	else
	{
		::PostMessage(hMainWnd, WM_USER_ADD_ATOM_MODE, 0, 0);  // 0은 비활성화 (선택)
	}
}


void ViewProgramTopMenu::OnBnClickedBtnDataTest()
{
	HWND hMainWnd = g_program->GetMVInstance()->GetSafeHwnd();
	CFileDialog dlg(TRUE, _T("txt"), nullptr,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Molecule Files (*.txt;*.mol)|*.txt;*.mol|All Files (*.*)|*.*||"),
		this);

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();

		if (::IsWindow(hMainWnd))
		{
			std::wstring* pStr = new std::wstring(path.GetString());
			::PostMessage(hMainWnd, WM_LOAD_MOLECULE_FILE, 0, reinterpret_cast<LPARAM>(pStr));
		}
	}
}
