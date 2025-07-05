// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "Sample.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(Sample, StyleDialog) 

// IDD_Sample을 새로 추가할 MFC 다이얼로그 리소스 아이디로 바꿔준다 (ex: IDD_VIEW_MAIN)
// 생성한 다이얼로그의 속성탭 ( 테두리 : NONE으로 변경, 스타일 : Child로 변경, 표시 : True로 변경(표시의 경우 모달리스로 전환이 필요한 경우 false로 놔둔다))
Sample::Sample(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_Sample, pParent)                                          
{
	m_pParent = pParent;
}

Sample::~Sample()
{
}

void Sample::UpdateLayerControl(int cx, int cy)
{
	
}

BOOL Sample::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetBackgroundColor(RGB(64, 64, 64));  // 배경 색상 
	
	StyleDialog::UpdateLayerControl();  	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Sample::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	StyleDialog::DoDataExchange(pDX);

}
