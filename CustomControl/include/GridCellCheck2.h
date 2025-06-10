#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include "GridCellCheck.h"

class __CUSTOM_CONTROL_CLASS__ CGridCellCheck2 : public CGridCellCheck
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellCheck2)

public:
    CGridCellCheck2();

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();

	BOOL Enable(BOOL bEnable = TRUE);
	BOOL GetEnabled();
    virtual void OnClick( CPoint PointCellRelative);

protected:
	//CRect GetCheckPlacement();
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

protected:
    BOOL  m_bEnabled;
};
