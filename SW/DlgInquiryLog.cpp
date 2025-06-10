// DlgInquiryLog.cpp : implementation file
//

#include "pch.h"
#include "SWDlg.h"
#include "DlgInquiryLog.h"
#include "afxdialogex.h"

#include "Log.h"
#include "../Base/Path.h"
#include "../Base/Utility.h"
#include "../CustomControl/include/SearchListCtrl.h"
#include "resource.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UM_SEARCH_END	(WM_USER + 100)

// DlgInquiryLog dialog

IMPLEMENT_DYNAMIC(DlgInquiryLog, CDialogEx)

DlgInquiryLog::DlgInquiryLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INQUIRY_LOG, pParent)
	, kill_request_(false)
{
}

DlgInquiryLog::~DlgInquiryLog()
{
	kill_request_ = true;
	if (serach_thread_.joinable())
	{
		serach_thread_.join();
	}
}

void DlgInquiryLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATE_START, date_time_start_);
	DDX_Control(pDX, IDC_DATE_END, date_time_end_);
	DDX_Control(pDX, IDC_BTN_SEARCH, button_search_);
	DDX_Control(pDX, IDC_BTN_EXPORT, button_export_);
}


BEGIN_MESSAGE_MAP(DlgInquiryLog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &DlgInquiryLog::OnBnClickedBtnSearch)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_START, &DlgInquiryLog::OnDtnDatetimechangeDateStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_END, &DlgInquiryLog::OnDtnDatetimechangeDateEnd)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &DlgInquiryLog::OnBnClickedBtnExport)

	ON_WM_SIZE()
	ON_MESSAGE(UM_SEARCH_END, &DlgInquiryLog::OnSearchEnd)
END_MESSAGE_MAP()


BOOL DlgInquiryLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	CRect rect;
	GetWindowRect(&rect); // 현재 대화 상자의 크기와 위치를 얻음
	MoveWindow(rect.left, rect.top, 600, 1000, TRUE); // 대화 상자의 크기를 변경


	SetBackgroundColor(RGB(64, 64, 64));

	m_lstLog.Create(CRect(0, 0, 1, 1), this);
	m_lstLog.SetStyle_FullrowSelect();
	m_lstLog.SetColumnFilterDisplayCountX(2);
	m_lstLog.AddColumn(_T("NO"), LVCFMT_RIGHT, 50);
	m_lstLog.AddColumn(_T("분류"), LVCFMT_LEFT, 100);
	m_lstLog.AddColumn(_T("시간"), LVCFMT_LEFT, 200);
	m_lstLog.AddColumn(_T("정보"), LVCFMT_LEFT, 200);


	UpdateLayerControl();

	CTime tmNow = CTime::GetCurrentTime();

	date_time_start_.SetTime(&tmNow);
	date_time_end_.SetTime(&tmNow);




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void DlgInquiryLog::UpdateLayerControl()
{
	CRect rect;

	GetClientRect(rect);

	UpdateLayerControl(rect.Width(), rect.Height());
}

void DlgInquiryLog::UpdateLayerControl(int cx, int cy)
{
	CRect rectSerach;
	button_search_.GetWindowRect(rectSerach);
	ScreenToClient(rectSerach);

	Utility::ControlAlignRight(cx - 10, button_export_);
	Utility::ControlAlignRight(cx - 20 - Utility::GetControlWidth(button_export_), button_search_);


	CRect rectLog;
	rectLog.top = rectSerach.bottom + 10;
	rectLog.left = 10;
	rectLog.right = cx - 10;
	rectLog.bottom = cy - 10;

	m_lstLog.MoveWindow(rectLog);
	long fixColumnWidth = 0;
	for (long columnIndex = 0; columnIndex < m_lstLog.GetColumnCount(); columnIndex++)
	{
		if (columnIndex != 3)
		{
			fixColumnWidth += m_lstLog.GetColumnWidth(columnIndex);
		}
	}

	m_lstLog.SetColumnWidth(3, rectLog.Width() - fixColumnWidth - 30);

}

void DlgInquiryLog::AsyncSearch(CTime tmStart, CTime tmEnd)
{
	Utility::SetLocale(_T("korean"));

	struct SubLogData
	{
		int type_;
		CString text_;

		SubLogData() = default;
		SubLogData(const SubLogData& obj)
			: type_(obj.type_)
			, text_(obj.text_)
		{
		}
	};

	found_logs_.clear();

	// 잠시 Log File Write 작업을 중단시킨다
	// 서로 파일을 접근하느라 로그 파일 오픈이 양쪽에서 실패할 수 있기 때문이다.

	g_log.WriteThread_Pause();

	for (auto time = tmEnd; time >= tmStart; time -= CTimeSpan(1, 0, 0, 0))
	{
		CString folderDate;
		folderDate.Format(_T("%s%04d%02d%02d\\"),
			Path::GetLogFolderPath(),
			time.GetYear(),
			time.GetMonth(),
			time.GetDay());

		auto date_key = time.GetYear() * 10000 + time.GetMonth() * 100 + time.GetDay();

		for (int hour = 24; hour >= 0; hour--)
		{
			CString folderHour;
			folderHour.Format(_T("%s%02d\\"), (LPCTSTR)folderDate, hour);

			// 시간별로 로그를 모은다
			std::map<int, std::vector<SubLogData>> timeLogs;

			for (int type = 0; type < (int)LogType::END; type++)
			{
				if (kill_request_) return;

				CString typeName = EnumToText((LogType)type);
				CString filePath;
				filePath.Format(_T("%s%s.txt"), (LPCTSTR)folderHour, (LPCTSTR)typeName);

				CStdioFile file;
				setlocale(LC_ALL, "korean");
				if (!file.Open(filePath, CStdioFile::modeRead)) continue;

				CString lineText;
				while (file.ReadString(lineText))
				{
					if (lineText.GetLength() <= 8) continue;
					CString timeText = lineText.Left(12);
					auto timeKey = _ttoi(timeText.Mid(0, 2)) * 10000000 + _ttoi(timeText.Mid(3, 2)) * 100000 + _ttoi(timeText.Mid(6, 2)) * 1000 + _ttoi(timeText.Mid(9, 3));

					SubLogData log;
					log.type_ = type;
					log.text_ = lineText.Mid(12);
					timeLogs[timeKey].push_back(log);
				}
			}

			auto it_logs = timeLogs.rbegin();
			while (it_logs != timeLogs.rend())
			{
				auto& logs = it_logs->second;

				for (auto& log : logs)
				{
					found_logs_.push_back(std::make_shared<LogData>(date_key, it_logs->first, log.type_, log.text_));
				}
				it_logs++;
			}
		}
	}

	g_log.WriteThread_Resume();	// Log Write 작업을 재개한다

	PostMessage(UM_SEARCH_END);
}

void DlgInquiryLog::OnDtnDatetimechangeDateStart(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	CTime tmStart;
	CTime tmEnd;
	date_time_start_.GetTime(tmStart);
	date_time_end_.GetTime(tmEnd);

	if (tmEnd < tmStart)
	{
		// End 범위가 Start범위 이전이면 End범위를 Start 범위로 만들어주자
		date_time_end_.SetTime(&tmStart);
	}

	*pResult = 0;
}

void DlgInquiryLog::OnDtnDatetimechangeDateEnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	CTime tmStart;
	CTime tmEnd;
	date_time_start_.GetTime(tmStart);
	date_time_end_.GetTime(tmEnd);

	if (tmStart > tmEnd)
	{
		// Start 범위가 End범위 이후면 Start범위를 End 범위로 만들어주자
		date_time_start_.SetTime(&tmEnd);
	}

	*pResult = 0;
}

void DlgInquiryLog::OnBnClickedBtnSearch()
{
	button_search_.EnableWindow(FALSE);
	button_export_.EnableWindow(FALSE); // 조회할때까지만 비활성화해서, 쓰레드 중복 입력 방지하는 것 같음.

	CTime tmStart;
	CTime tmEnd;
	date_time_start_.GetTime(tmStart);
	date_time_end_.GetTime(tmEnd);

	serach_thread_ = std::thread([=]() { AsyncSearch(tmStart, tmEnd); });
}

void DlgInquiryLog::OnBnClickedBtnExport()
{
	LPCTSTR filter = _T("CSV files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), _T("*.csv"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() != IDOK) return;

	m_lstLog.ExportCSV(dlg.GetPathName());
}

void DlgInquiryLog::OnSize(UINT nType, int cx, int cy) // 윈도우사이즈 초기화용 
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_lstLog.GetSafeHwnd() == NULL) return;

	UpdateLayerControl(cx, cy);
}

LRESULT DlgInquiryLog::OnSearchEnd(WPARAM, LPARAM) // 검색 완료 
{
	if (serach_thread_.joinable())
	{
		serach_thread_.join();
	}

	auto item_data = std::make_shared<SearchListCtrl::ListItemData>();

	item_data->item_text_.resize(found_logs_.size());

	int row_index = 0;

	for (auto log : found_logs_)
	{
		auto date_key = log->date_key_;
		auto time_key = log->time_key_;

		auto year = date_key / 10000;
		auto month = (date_key % 10000) / 100;
		auto day = date_key % 100;

		auto hour = time_key / 10000000;
		auto min = (time_key % 10000000) / 100000;
		auto sec = (time_key % 100000) / 1000;
		auto milc = time_key % 1000;


		CString fieldTime;
		fieldTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03ld"),
			year, month, day,
			hour, min, sec, milc);

		auto& row_data = item_data->item_text_[row_index];
		row_data[0] = Utility::FormatString(_T("%d"), row_index + 1);
		row_data[1] = EnumToText((LogType)log->type_);
		row_data[2] = fieldTime;
		row_data[3] = log->text_;

		row_index++;
	}

	m_lstLog.SetItemData(item_data);

	button_search_.EnableWindow(TRUE);
	button_export_.EnableWindow(TRUE);

	return 0;
}
