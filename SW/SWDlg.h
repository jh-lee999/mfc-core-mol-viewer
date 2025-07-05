#pragma once
#include "../CustomControl/include/StyleDialog.h"

class ViewMain;
class ViewProgramTopMenu;
class ViewProgramLog;
class ViewMenu1;

// CSWDlg 대화 상자
class CSWDlg : public StyleDialog
{
// 생성입니다.
public:
	CSWDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void RequestProgramExit();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	void UpdateLayerControl(int cx, int cy) override;

	void ProcPostMessage(PostCommandType command, std::shared_ptr<PostObj> obj);
	void ProcPostMessage(long command, std::shared_ptr<PostObj> obj) override;
	ViewMain* view_main;
	ViewProgramTopMenu* top_menu;
	ViewProgramLog* view_log;
	ViewMenu1* menu_1;
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	void ToggleWindow(CWnd* window);
	void HideAllWindows();
	int top_menu_height = 0;
	afx_msg void OnBnClickedMainBtnMenu1();
	void SetBackGroundPos(int x, int y, int cx, int cy);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ToggleFullscreen();
	bool m_bIsFullscreen = false;
	ViewMain* GetMVInstance() { return view_main; };
};

extern CSWDlg* g_program;
extern StyleDialog* g_postProc;