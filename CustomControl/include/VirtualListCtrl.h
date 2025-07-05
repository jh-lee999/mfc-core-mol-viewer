#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

// VirtualListCtrl

typedef CString (* CALLBACK_VIRTUAL_LIST_ITEM)(LPVOID user_data, int32_t row_index, int32_t column_index);

class __CUSTOM_CONTROL_CLASS__ VirtualListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(VirtualListCtrl)

public:
	VirtualListCtrl(LPVOID user_data, CALLBACK_VIRTUAL_LIST_ITEM callback_virtual);
	virtual ~VirtualListCtrl();

	void SetEnableSelectChanged(bool enable);
	void SetEvent_SelectChanged(CWnd* target, UINT message);

protected:
	LPVOID user_data_;
	CALLBACK_VIRTUAL_LIST_ITEM callback_virtual_;
	bool	selectChanged_enable_;
	UINT	selectChanged_msg_;
	CWnd*	selectChanged_target_;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
};
