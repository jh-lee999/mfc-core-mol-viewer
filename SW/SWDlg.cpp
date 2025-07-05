
// SWDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SW.h"
#include "SWDlg.h"
#include "afxdialogex.h"
#include "Log.h"

#include "ViewMain.h"
#include "ViewProgramLog.h"
#include "ViewProgramTopMenu.h"
#include "ViewMenu1.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSWDlg* g_program = nullptr;
StyleDialog* g_postProc = nullptr;


CSWDlg::CSWDlg(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_SW_DIALOG, pParent)
{
	top_menu = new ViewProgramTopMenu(this);


	// 메뉴 추가시 작업해야하는 사항 
	view_main = new ViewMain(this);	
	menu_1 = new ViewMenu1(this);

	g_program = this;
	g_postProc = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSWDlg::RequestProgramExit()
{
	if (MessageBox(_T("프로그램을 종료하시겠습니까?"), _T("Drilling Machine"), MB_YESNO) != IDYES) return;

	StyleDialog::Close();

}

void CSWDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
}

void CSWDlg::UpdateLayerControl(int cx, int cy)
{
	
	int log_menu_height = 0;
	top_menu_height = 50;


	// 메뉴 추가시 작업해야하는 사항 
	view_main->MoveWindow(0, top_menu_height, cx, cy- top_menu_height- log_menu_height);
	menu_1->MoveWindow(0, top_menu_height, cx, cy - top_menu_height - log_menu_height);


	top_menu->MoveWindow(0, 0, cx, top_menu_height);
}

void CSWDlg::ProcPostMessage(PostCommandType command, std::shared_ptr<PostObj> obj)
{
	switch (command)
	{
	}
}


void CSWDlg::ProcPostMessage(long command, std::shared_ptr<PostObj> obj)
{
	ProcPostMessage((PostCommandType)command, obj);
}

BEGIN_MESSAGE_MAP(CSWDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MAIN_BTN_MENU1, &CSWDlg::OnBnClickedMainBtnMenu1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSWDlg 메시지 처리기

BOOL CSWDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();

	//MoveWindow(Utility::GetMonitorRegion(0)); // 화면 크기 조절

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	m_hIcon = AfxGetApp()->LoadIcon(IDI_TITLE);
	
	SetWindowTextW(_T("Molecule Viewer SW"));

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	g_log.Write(LogType::Debug, _T("[SW Start]"));

	// 메뉴 추가시 작업해야하는 사항 
	view_main->Create();
	top_menu->Create();
	menu_1->Create();

	StyleDialog::UpdateLayerControl();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSWDlg::SetBackGroundPos(int x, int y , int cx, int cy){
	MoveWindow(x, y, cx, cy);
	StyleDialog::UpdateLayerControl();
	Invalidate(false);
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSWDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSWDlg::OnClose()
{
	EndDialog(IDOK);
;
	StyleDialog::OnClose();
}


void CSWDlg::ToggleWindow(CWnd* window)
{
	if (window->IsWindowVisible())
	{
		window->ShowWindow(SW_HIDE);
		window->SetFocus();
		view_main->ShowWindow(SW_SHOW);
	}
	else
	{
		HideAllWindows();
		window->ShowWindow(SW_SHOW);
	}
}

void CSWDlg::HideAllWindows()
{
	// 메뉴 추가시 작업해야하는 사항 
	view_main->ShowWindow(SW_HIDE);
	menu_1->ShowWindow(SW_HIDE);
	// 새로 생성하는 모달리스 관련 변수들을 ->ShowWindow(SW_HIDE) 해줘야함.
	//ex: i_track->ShowWindow(SW_HIDE);
}


// 메뉴 추가시 작업해야하는 사항  -> 버튼 새로 만든 후 ToggleWindow 함수 버튼을 생성후, ViewProgramTopMenu에서 해당 버튼을 불러오게 하면 됨 
void CSWDlg::OnBnClickedMainBtnMenu1()
{
	ToggleWindow(menu_1);
}



void CSWDlg::OnSize(UINT nType, int cx, int cy)
{
	StyleDialog::OnSize(nType, cx, cy);

	std::cout << "nType: " << nType << ", m_bIsFullscreen: " << m_bIsFullscreen << "\n";

	if (nType == SIZE_MAXIMIZED && !m_bIsFullscreen)
	{
		std::cout << ">> 최대화 감지\n";
		m_bIsFullscreen = true;
		ToggleFullscreen(); // 여기서 강제 레이아웃 갱신
	}
	else if (nType == SIZE_RESTORED && m_bIsFullscreen)
	{
		std::cout << ">> 복원 감지\n";
		m_bIsFullscreen = false;
		ToggleFullscreen(); // 여기서도 강제 레이아웃 갱신
	}
}






void CSWDlg::ToggleFullscreen()
{
	HWND hwnd = GetSafeHwnd();

	// 현재 클라이언트 크기 얻기
	CRect rc;
	GetWindowRect(&rc);

	::SetWindowPos(GetSafeHwnd(), HWND_TOP,
		rc.left, rc.top, rc.Width(), rc.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);

}

