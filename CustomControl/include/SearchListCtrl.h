#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include "VirtualListCtrl.h"
#include "StyleButton.h"

#include <afxmt.h>
#include <thread>
#include <map>
#include <vector>

class __CUSTOM_CONTROL_CLASS__ SearchListCtrl : public CStatic
{
	DECLARE_DYNAMIC(SearchListCtrl)

public:
	SearchListCtrl();
	~SearchListCtrl();

	struct ListItemData
	{
		std::vector<std::map<int32_t, CString>> item_text_;
	};

	int		GetColumnCount() const;
	CString	GetColumnName(int columnIndex) const;
	int		GetColumnWidth(int columnIndex) const;
	int		GetItemCount() const;
	void	GetSelectedItemIndices(std::vector<int>& o_indices);
	bool	ExportCSV(LPCTSTR filePath) const;

	bool	Create(const RECT& rect, CWnd* parent);
	void	AddColumn(LPCTSTR name, int format = 0, int width = -1);
	void	SetColumnWidth(int columnIndex, int cx);
	void	SetColumnFilterDisplayCountX(int count);
	void	SetEvent_SelectChanged(CWnd* target, UINT message);

	CString	GetItemText(int32_t row_index, int32_t column_index) const;
	void	SetItemData(std::shared_ptr<ListItemData> data);

	void	SetCurSel(int selectItemIndex);
	void	SetStyle_FullrowSelect();

protected:
	void UpdateLayerControl();
	void UpdateList();
	void ThreadProc();

	static CString CallbackVirtualListItem(LPVOID user_data, int32_t row_index, int32_t column_index);

	class ColumnFilter
	{
	public:
		ColumnFilter(SearchListCtrl* parent, int columnIndex, LPCTSTR columnName);
		void MoveWindow(int x, int y, int width, int height);
		void UpdateText();

		CString GetText() const;

		int m_index;
		CEdit m_edit;
	private:
		CStatic m_title;
		CString m_editText;
		mutable CCriticalSection m_csText;
	};

	std::vector<ColumnFilter *> m_columnFilters;
	int m_columnFilterDisplayCountX;

	mutable CCriticalSection cs_list_data_;
	std::shared_ptr<ListItemData> list_full_data_;
	std::shared_ptr<ListItemData> list_data_;
	VirtualListCtrl list_;

	std::thread m_thread;
	CEvent m_eventUpdate;
	CEvent m_eventEnd;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateSearchList(WPARAM, LPARAM);
	afx_msg void OnChangedFilterEdit();
};
