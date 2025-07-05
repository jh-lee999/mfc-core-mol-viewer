#pragma once
#include "../CustomControl/include/StyleDialog.h"
#include "../CustomControl/include/StyleButton.h"
#include "../CustomControl/include/StyleStatic.h"
#include "../CustomControl/include/StyleStaticCheckBox.h"
#include <vector>


class ViewProgramTopMenu : public StyleDialog
{
	DECLARE_DYNAMIC(ViewProgramTopMenu)

public:
	ViewProgramTopMenu(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ViewProgramTopMenu();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_PROGRAM_TOP_MENU   
	}; 
#endif

protected:
	CFont m_fontRecipeName;
	virtual void DoDataExchange(CDataExchange* pDX);
	void UpdateLayerControl(int cx, int cy) override;
	DECLARE_MESSAGE_MAP()
public:

private:
	CWnd* m_pParent;     // 부모 윈도우의 포인터
	bool m_bDragging = false;    // 윈도우 드래그 중인지 여부를 나타내는 플래그

	CPoint m_ptDragPos;  // 드래그 시작 시의 마우스 위치
public:

	virtual BOOL OnInitDialog();	

	afx_msg void OnBnClickedAddatom();
	afx_msg void OnBnClickedBtnDataTest();
};
