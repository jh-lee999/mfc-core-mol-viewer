#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <vector>

class StyleStatic;

// StyleStaticGroup

class __CUSTOM_CONTROL_CLASS__ StyleStaticGroup : public CStatic
{
	DECLARE_DYNAMIC(StyleStaticGroup)

public:
	StyleStaticGroup();
	virtual ~StyleStaticGroup();

	void Merge(long leftIndex, long rightIndex);
	bool SetArray(long count);
	void SetAll_AlignLeft();
	void SetAll_AlignCenter();
	void SetAll_BackgroundColor(COLORREF color);
	void SetAll_ForeColor(COLORREF color);

	StyleStatic* Get(long index);

protected:
	std::vector<StyleStatic*> m_items;
	void UpdateLayerControl();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
