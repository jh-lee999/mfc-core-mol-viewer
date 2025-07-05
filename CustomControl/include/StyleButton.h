#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

// Return values
#define	BTNST_OK						0
#define	BTNST_INVALIDRESOURCE			1
#define	BTNST_FAILEDMASK				2
#define	BTNST_INVALIDINDEX				3
#define	BTNST_INVALIDALIGN				4
#define	BTNST_BADPARAM					5
#define	BTNST_INVALIDPRESSEDSTYLE		6
#define USER_BTN_DN		WM_USER + 1000
#define USER_BTN_UP		WM_USER + 1001 // 버튼 누르고 있을때 활성화 시킬 목적 

class __CUSTOM_CONTROL_CLASS__ StyleButton : public CButton
{
public:
	StyleButton();
	~StyleButton();

	enum {
		BTNST_COLOR_BK_IN = 0,		// Background color when mouse is INside
		BTNST_COLOR_FG_IN,				// Text color when mouse is INside
		BTNST_COLOR_BK_OUT,				// Background color when mouse is OUTside
		BTNST_COLOR_FG_OUT,				// Text color when mouse is OUTside
		BTNST_COLOR_BK_FOCUS,			// Background color when the button is focused
		BTNST_COLOR_FG_FOCUS,			// Text color when the button is focused

		BTNST_MAX_COLORS
	};

	enum {
		BTNST_PRESSED_LEFTRIGHT = 0,	// Pressed style from left to right (as usual)
		BTNST_PRESSED_TOPBOTTOM			// Pressed style from top to bottom
	};

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StyleButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	DWORD SetDefaultColors(BOOL bRepaint = TRUE);
	DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	DWORD GetColor(BYTE byColorIndex, COLORREF* crpColor);
	DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 추후 추가 
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	int GetCheck();

	void DrawTransparent(BOOL bRepaint = FALSE);
	DWORD SetBk(CDC* pDC);

	BOOL GetDefault();

	DWORD SetPressedStyle(BYTE byStyle, BOOL bRepaint = TRUE);
	DWORD SetIcon(int nIconIn);

	BOOL	m_bShowDisabledBitmap;
	POINT	m_ptImageOrg;
	POINT	m_ptPressedOffset;

protected:
	//{{AFX_MSG(StyleButton)

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnClicked();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnCancelMode();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	COLORREF DarkenColor(COLORREF crColor, double dFactor);
	virtual DWORD OnDrawBackground(CDC* pDC, CRect* pRect);
	virtual DWORD OnDrawBorder(CDC* pDC, CRect* pRect);

	BOOL		m_bMouseOnButton;	// Is mouse over the button?
	BOOL		m_bDrawTransparent;	// Draw transparent?
	BOOL		m_bIsPressed;		// Is button pressed?
	BOOL		m_bIsFocused;		// Is button focused?
	BOOL		m_bIsDisabled;		// Is button disabled?
	BOOL		m_bIsDefault;		// Is default button?
	BOOL		m_bIsCheckBox;		// Is the button a checkbox?
	COLORREF	m_crColors[BTNST_MAX_COLORS];	// Colors to be used
	HWND		m_hParentWndMenu;	// Handle to window for menu selection
	BOOL		m_bMenuDisplayed;	// Is menu displayed ?

private:
	DWORD OffsetColor(BYTE byColorIndex, short shOffset);

	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	short m_nPush;
	void CancelHover();
	void FreeResources(BOOL bCheckForNULL = TRUE);
	void PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);
	virtual void DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	virtual void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	void PaintBk(CDC* pDC);

	CDC			m_dcBk;
	CBitmap		m_bmpBk;
	CBitmap* m_pbmpOldBk;
	int			m_nCheck;			// Current value for checkbox
	UINT		m_nTypeStyle;		// Button style

#pragma pack(1)
	typedef struct _STRUCT_ICONS
	{
		HICON		hIcon;			// Handle to icon
		DWORD		dwWidth;		// Width of icon
		DWORD		dwHeight;		// Height of icon
	} STRUCT_ICONS;
#pragma pack()

	STRUCT_ICONS	m_csIcon;

	DECLARE_MESSAGE_MAP()
};
