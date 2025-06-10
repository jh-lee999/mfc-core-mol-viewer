#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

/////////////////////////////////////////////////////////////////////////////
// GridCellCombo.h : header file
//
// MFC Grid Control - Grid combo cell class header file
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10
//
//////////////////////////////////////////////////////////////////////


#include "GridCell.h"

class __CUSTOM_CONTROL_CLASS__ CGridCellCombo : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellCombo)

public:
    CGridCellCombo();
	CGridCellCombo(DWORD dwStyle);

// editing cells
public:
    virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual CWnd* GetEditWnd() const;
    virtual void  EndEdit();

// Operations
public:
	virtual CSize GetCellExtent(CDC* pDC);

// CGridCellCombo specific calls
public:
    const CStringArray& GetOptions() const;
    void  SetOptions(const CStringArray& ar);
    void  SetStyle(DWORD dwStyle)           { m_dwStyle = dwStyle; }
    DWORD GetStyle()                        { return m_dwStyle;    }

protected:
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

    CStringArray m_Strings;
    DWORD        m_dwStyle;
};
