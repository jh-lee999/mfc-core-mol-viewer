#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <map>
#include <vector>
#include "StyleStatic.h"

class __CUSTOM_CONTROL_CLASS__ StyleStaticCheckBox : public CStatic
{
	DECLARE_DYNAMIC(StyleStaticCheckBox)

public:
	StyleStaticCheckBox();
	virtual ~StyleStaticCheckBox();

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

	struct GridInfo
	{
		GridInfo() : m_state(false), m_color(RGB(255, 255, 255)) {}

		bool m_state;
		COLORREF m_color;
	};

	COLORREF m_backgroundColor;
	HBITMAP m_backgroundImage;
	long	m_backgroundImage_MarinX;
	long	m_backgroundImage_MarinY;
	DWORD	m_backgroundImage_valignStyle;
	bool m_enableBorder;

	std::map<long, TextInfo> m_textInfos;
	std::vector<std::vector<bool>> m_gridStates;

	DECLARE_MESSAGE_MAP()
	void PaintText(CDC& dc, const TextInfo& textInfo);
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();

	void SetStaticState(UINT nFlags, CPoint point, int grid_index);
	void CreateCheckStatic(int row, int col, int grid_index, COLORREF bkcolor,COLORREF activecolor, HWND hParentWnd);

	std::vector<StyleStatic*> m_subControls;
	int grid_row_;
	int grid_col_;
	int gridIndex_;
	CRect m_gridRect_;
	COLORREF m_gridBkColor_;
	COLORREF m_gridActiveColor_;

	void UpdateGridState(int row, int col, COLORREF color);
	void AdjustGridSize();
	bool GetGridState(int row, int col);
	void InitGridState(std::vector<std::vector<bool>> gridStates);
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint);

private:
	HWND m_hParentWnd; // 부모 윈도우 핸들
};
