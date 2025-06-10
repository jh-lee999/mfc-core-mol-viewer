#include "pch.h"
#include "SWDlg.h"
#include "resource.h"
#include "ViewProgramLog.h"

// ViewProgramLog 대화 상자
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ViewProgramLog dialog

IMPLEMENT_DYNAMIC(ViewProgramLog, StyleDialog)

ViewProgramLog::ViewProgramLog(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_VIEW_PROGRAM_LOG, pParent)
{
}

ViewProgramLog::~ViewProgramLog()
{
}

void ViewProgramLog::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_LOG, tab_log_);
}

BEGIN_MESSAGE_MAP(ViewProgramLog, StyleDialog)
	ON_MESSAGE(UM_APPENDED_LOG, OnAppendedLog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOG, &ViewProgramLog::OnTcnSelchangeTabLog)
END_MESSAGE_MAP()

// ViewProgramLog message handlers

BOOL ViewProgramLog::OnInitDialog()
{
	StyleDialog::OnInitDialog();

	SetBackgroundColor(RGB(230, 230, 230));

	log_box_all.Create(WS_CHILD | WS_BORDER | WS_VSCROLL, CRect(0, 0, 1, 1), this, IDC_STATIC);


	for (long type = 0; type < LogType::END; type++)
	{
		log_box_[type].Create(WS_CHILD | WS_BORDER | WS_VSCROLL, CRect(0, 0, 1, 1), this, IDC_STATIC);
	}

	UpdateLogTabs();

	StyleDialog::UpdateLayerControl();

	g_log.SetWnd(GetSafeHwnd());
	OnTcnSelchangeTabLog(nullptr, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT ViewProgramLog::OnAppendedLog(WPARAM, LPARAM)
{
	std::queue<std::pair<LogType, CString>> logList;
	g_log.Pop(logList);
	if (logList.size() == 0) return 0;

	log_box_all.SetRedraw(FALSE);

	for (long type = 0; type < LogType::END; type++)
	{
		log_box_[type].SetRedraw(FALSE);
	}

	while (logList.size())
	{
		auto log = logList.front();
		logList.pop();

		PushLogToListBox(log_box_[log.first], log.second);
		PushLogToListBox(log_box_all, log.second);
	}

	for (long type = 0; type < LogType::END; type++)
	{
		log_box_[type].SetRedraw(TRUE);
	}

	log_box_all.SetRedraw(TRUE);

	long tabIndex = tab_log_.GetCurSel();
	return 0;
}

void ViewProgramLog::OnTcnSelchangeTabLog(NMHDR* pNMHDR, LRESULT* pResult)
{
	long tabIndex = tab_log_.GetCurSel();
	long logType = tabIndex >= 0 ? m_tabTypes[tabIndex] : LogType::END;

	log_box_all.ShowWindow(logType == LogType::END ? SW_SHOW : SW_HIDE);

	for (long type = 0; type < LogType::END; type++)
	{
		log_box_[type].ShowWindow(logType == type ? SW_SHOW : SW_HIDE);
	}

	if (pResult)
	{
		*pResult = 0;
	}
}

void ViewProgramLog::ChangedLogOption()
{
	UpdateLogTabs();
}

void ViewProgramLog::PushLogToListBox(CListBox& logBox, LPCTSTR text)
{
	bool autoScrolling = false;

	if (logBox.GetCurSel() == logBox.GetCount() - 1)
	{
		// 커서가 마지막 줄에 있을 때만 자동 스크롤
		autoScrolling = true;
	}

	if (logBox.GetCount() > 100)
	{
		// 오래된 로그부터 제거
		logBox.DeleteString(0);
	}

	logBox.AddString(text);

	if (autoScrolling)
	{
		logBox.SetTopIndex(logBox.GetCount() - 1);
		logBox.SetCurSel(logBox.GetCount() - 1);
	}
}

void ViewProgramLog::UpdateLogTabs()
{
	LogType selectTabType = LogType::END;
	if (m_tabTypes.size()) selectTabType = m_tabTypes[tab_log_.GetCurSel()];

	m_tabTypes.clear();
	m_tabTypes.push_back(LogType::END);

	tab_log_.DeleteAllItems();
	tab_log_.InsertItem(0, _T("ALL"));

	long selectTabIndex = 0;

	for (long typeIndex = 0; typeIndex < LogType::END; typeIndex++)
	{
		auto type = (LogType)typeIndex;

		long tabIndex = tab_log_.GetItemCount();
		tab_log_.InsertItem(tabIndex, EnumToText(type));
		m_tabTypes.push_back(type);

		if (selectTabType == type)
		{
			selectTabIndex = tabIndex;
		}
	}

	tab_log_.SetCurSel(selectTabIndex);
	OnTcnSelchangeTabLog(nullptr, nullptr);
}

void ViewProgramLog::UpdateLayerControl(int cx, int cy)
{
	

	tab_log_.MoveWindow(0, 0, cx-10, cy-10);

	CRect tabInnerRegion(5, 5, cx - 10, cy - 20);

	log_box_all.MoveWindow(tabInnerRegion);

	for (long type = 0; type < LogType::END; type++)
	{
		log_box_[type].MoveWindow(tabInnerRegion);
	}
}
