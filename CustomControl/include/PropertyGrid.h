#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

class CGridCtrl;
class PropertyGridLinks;

#include "PropertyGridItem.h"
#include "StyleEdit.h"
#include "StyleStatic.h"
#include <vector>

// PropertyGrid

typedef void (*CallbackPropertyGridItemChanged)(LPVOID userData, LPCTSTR parameterName);
typedef void (*CallbackPropertyGridButtonClicked)(LPVOID userData, long command);

class __CUSTOM_CONTROL_CLASS__ PropertyGrid : public CStatic
{
	DECLARE_DYNAMIC(PropertyGrid)

public:
	PropertyGrid();
	virtual ~PropertyGrid();

	bool Create(const RECT& rect, CWnd* parent);

	void SetCallbackPropertyGridItemChanged(LPVOID user_data, CallbackPropertyGridItemChanged callback);
	void SetDescriptionHeight(long descriptionHeight);
	void ResetItems();
	void AddCategoryLine(LPCTSTR categoryName);
	PropertyGridItem* LockAddItem(LPCTSTR parameterName, CString& io_value, CStringArray* options= nullptr);
	PropertyGridItem* AddItem(LPCTSTR parameterName, CString& io_value, CStringArray* options = nullptr);
	PropertyGridItem* AddItem(LPCTSTR parameterName, bool& io_value);
	PropertyGridItem* AddItem(LPCTSTR parameterName, int32_t& io_value, bool isDefault);
	PropertyGridItem* AddItem(LPCTSTR parameterName, float& io_value);
	PropertyGridItem* AddItem(LPCTSTR parameterName, long& io_value, CStringArray* options = nullptr);
	PropertyGridItem* AddItem(LPCTSTR parameterName, bool& io_value, CStringArray* options);
	PropertyGridItem* AddItem(LPCTSTR parameterName, int32_t& io_value, CStringArray* options = nullptr);
	PropertyGridItem* AddItemButton(LPCTSTR parameterName, LPCTSTR buttonName, LPVOID userData, long command, CallbackPropertyGridButtonClicked function);
	PropertyGridItem* AddItemButtons(LPCTSTR parameterName, const std::vector<CString>& buttonNames, LPVOID userData, long startCommand, CallbackPropertyGridButtonClicked function);

protected:
	bool m_created;
	long m_descriptionHeight;
	LOGFONT m_fontCategory;
	CImageList m_imageList;
	CString m_propertyNameHead;

	CGridCtrl* m_gridView;
	StyleStatic m_descriptionHead;
	StyleEdit m_description;

	PropertyGridLinks* m_link;
	std::vector<PropertyGridItem*> m_items;

	void UpdateColumnWidth();
	void UpdateLayerControl();

	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGridItemSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGridItemStateChanged(NMHDR* pNMHDR, LRESULT* pResult);
};
