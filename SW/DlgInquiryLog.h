#pragma once
#include "../CustomControl/include/SearchListCtrl.h"
#include "../Base/Utility.h"
#include <thread>
#include <vector>

// DlgInquiryLog dialog

class DlgInquiryLog : public CDialogEx
{
	DECLARE_DYNAMIC(DlgInquiryLog)

public:
	DlgInquiryLog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DlgInquiryLog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INQUIRY_LOG };
#endif

protected:
	void UpdateLayerControl();
	void UpdateLayerControl(int cx, int cy);
	void AsyncSearch(CTime tmStart, CTime tmEnd);
	struct LogData
	{
		int32_t date_key_;
		int32_t time_key_;
		int32_t type_;
		CString text_;

		LogData(int32_t date_key, int32_t time_key, int32_t type, LPCTSTR text)
			: date_key_(date_key)
			, time_key_(time_key)
			, type_(type)
			, text_(text)
		{
		}
	};

	std::vector<std::shared_ptr<LogData>> found_logs_;	// Date/Time/Log
	bool kill_request_;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

	SearchListCtrl m_lstLog;

	std::thread serach_thread_;

	afx_msg void OnDtnDatetimechangeDateStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangeDateEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSearchEnd(WPARAM, LPARAM);
	CDateTimeCtrl date_time_start_;
	CDateTimeCtrl date_time_end_;
	CButton button_search_;
	CButton button_export_;
};
