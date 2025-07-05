#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <map>

// StyleStatic

class __CUSTOM_CONTROL_CLASS__ StyleStatic : public CStatic
{
	DECLARE_DYNAMIC(StyleStatic)

public:
	StyleStatic();
	virtual ~StyleStatic();

	void EnableBorderLine(bool enable);
	void ResetSubLayers();
	void SetAlignLeft(long layer = -1);
	void SetAlignRight(long layer = -1);
	void SetAlignCenter(long layer = -1);
	void SetAlignBottom(long layer = -1);
	void SetBackgroundColor(COLORREF color);
	void SetBackgroundImage(HBITMAP image, long marginX = 0, long marginY = 0);
	void SetBackgroundImageAlignTop();
	void SetForeColor(COLORREF color, long layer = -1);
	void SetFont(CFont* font, long layer = -1);
	void SetWindowText(LPCTSTR text, long layer = -1);

	bool m_state;

protected:
	class TextInfo
	{
	public:
		TextInfo();

		COLORREF m_foreColor;
		CFont* m_externalFont;
		CString m_text;
		DWORD m_alignStyle;
		DWORD m_valignStyle;
	};

	COLORREF m_backgroundColor;
	HBITMAP m_backgroundImage;
	long	m_backgroundImage_MarinX;
	long	m_backgroundImage_MarinY;
	DWORD	m_backgroundImage_valignStyle;
	bool m_enableBorder;

	std::map<long, TextInfo> m_textInfos;

	void PaintText(CDC& dc, const TextInfo& textInfo);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();

	void ClickStatic(UINT nFlags, CPoint point);
};
