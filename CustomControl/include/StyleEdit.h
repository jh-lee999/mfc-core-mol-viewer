#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

// StyleEdit

class __CUSTOM_CONTROL_CLASS__ StyleEdit : public CEdit
{
	DECLARE_DYNAMIC(StyleEdit)

public:
	StyleEdit();
	virtual ~StyleEdit();

	void SetBackgroundColor(COLORREF color);
	void SetForeColor(COLORREF color);

protected:
	COLORREF m_foreColor;
	CBrush m_backgroundBrush;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


