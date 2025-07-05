#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include "atlimage.h"
#include "GridCell.h"

// #pragma once
// #include "gridcell.h"
// 
// class  :
// 	public CGridCell
// {
// public:
// 	CGridCellImage(void);
// 	~CGridCellImage(void);
// };
class __CUSTOM_CONTROL_CLASS__ CGridCellImage : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CGridCellImage)

public:
	CGridCellImage();

public:
// 	BOOL SetCheck(BOOL bChecked = TRUE);
// 	BOOL GetCheck();
// 
// 	// Operations
// 	virtual CSize GetCellExtent(CDC* pDC);
// 	virtual void OnClick( CPoint PointCellRelative);
// 	virtual BOOL GetTextRect( LPRECT pRect);
// 	CRect GetCheckPlacement();
	void SetCellImage(LPCTSTR szImagePathName);
protected:
	
	CImage m_image;
	CString m_strImagePathName;

	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);


	
};

/////////////////////////////////////////////////////////////////////////////
