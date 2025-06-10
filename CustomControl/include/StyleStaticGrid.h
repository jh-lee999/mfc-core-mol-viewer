#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <vector>

class StyleStatic;
class StyleStaticGroup;

// StyleStaticGrid

class __CUSTOM_CONTROL_CLASS__ StyleStaticGrid : public CStatic
{
	DECLARE_DYNAMIC(StyleStaticGrid)

public:
	StyleStaticGrid();
	virtual ~StyleStaticGrid();

	bool SetArray(long cx, long cy);
	void SetAll_AlignLeft();
	void SetAll_AlignCenter();
	void SetAll_BackgroundColor(COLORREF color);
	void SetColumn_BackgroundColor(long x, COLORREF color);
	void SetColumn_ForeColor(long x, COLORREF color);
	void SetRow_BackgroundColor(long y, COLORREF color);
	void SetRow_ForeColor(long y, COLORREF color);

	StyleStatic* Get(long x, long y);
	StyleStaticGroup* GetRow(long y);

protected:
	std::vector<StyleStaticGroup*> m_rows;

	void UpdateLayerControl();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
