#pragma once
#include "../CustomControl/include/StyleDialog.h"
#include "../CustomControl/include/StyleButton.h"
#include "../CustomControl/include/StyleStatic.h"
#include "../CustomControl/include/StyleStaticCheckBox.h"
#include <vector>


class ViewMenu1 : public StyleDialog
{
	DECLARE_DYNAMIC(ViewMenu1)

public:
	ViewMenu1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ViewMenu1();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODALESS_MENU1   // 해당 부분에 'IDD = ' 뒤의 값으로 다이얼로그 리소스 아이디 입력(ex: IDD_VIEW_MANI) 
	}; 
#endif

protected:
	CFont m_fontRecipeName;
	virtual void DoDataExchange(CDataExchange* pDX);
	void UpdateLayerControl(int cx, int cy) override;
public:

private:
	CWnd* m_pParent;     // 부모 윈도우의 포인터
public:

	virtual BOOL OnInitDialog();

};
