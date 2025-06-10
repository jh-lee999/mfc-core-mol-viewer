#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <map>

// SortColumnListCtrl

class __CUSTOM_CONTROL_CLASS__ SortColumnListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(SortColumnListCtrl)

public:
	SortColumnListCtrl();
	virtual ~SortColumnListCtrl();

	void SetColumnSortingEnable(int columnIndex, bool isNumber);
	void SetColumnSortingDisable(int columnIndex);
	void SortColumnDisplayHide();
	void SetEnableSelectChanged(bool enable);
	void SetEvent_SelectChanged(CWnd* target, UINT message);

protected:
	static int CALLBACK SortCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SortColumn(int columnIndex);

	bool	m_selectChanged_enable;
	UINT	m_selectChanged_msg;
	CWnd*	m_selectChanged_target;

	int m_currentSortColumnIndex;
	bool m_ascending;
	std::map<int, bool> m_sortColumns;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
public:
	afx_msg BOOL OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
};
