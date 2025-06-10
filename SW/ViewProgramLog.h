#pragma once
#include "Log.h"
#include "../CustomControl/include/StyleDialog.h"
#include "afxwin.h"
#include <vector>

// ViewProgramLog dialog

class ViewProgramLog : public StyleDialog
{
	DECLARE_DYNAMIC(ViewProgramLog)

public:
	ViewProgramLog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ViewProgramLog();

	void ChangedLogOption();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_PROGRAM_LOG};
#endif

protected:
	std::vector<LogType> m_tabTypes;

	void PushLogToListBox(CListBox& logBox, LPCTSTR text);
	void UpdateLogTabs();
	void UpdateLayerControl(int cx, int cy) override;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl tab_log_;
	CListBox log_box_all;
	CListBox log_box_[LogType::END];

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAppendedLog(WPARAM, LPARAM);
	afx_msg void OnTcnSelchangeTabLog(NMHDR* pNMHDR, LRESULT* pResult);
};
