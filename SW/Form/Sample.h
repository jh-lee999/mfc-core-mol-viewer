#pragma once
#include "../CustomControl/StyleDialog.h"
#include "../CustomControl/StyleButton.h"
#include "../CustomControl/StyleStatic.h"
#include "../CustomControl/StyleStaticCheckBox.h"
#include <vector>


class Sample : public StyleDialog
{
	DECLARE_DYNAMIC(Sample)

public:
	Sample(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Sample();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Sample   // 해당 부분에 IDD_VIEW_MAIN 값을 cpp에서 바꿨던 값으로 수정해야함 
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
