#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include "GridCell.h"
#include <vector>

class __CUSTOM_CONTROL_CLASS__ CGridCellButtons : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellButtons)

public:
    CGridCellButtons();

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();

    long m_selectedButtonIndex;

    void AddButton(LPCTSTR buttonName);
    BOOL Enable(BOOL bEnable = TRUE);
    BOOL GetEnabled();
    virtual void OnClick(CPoint PointCellRelative);
    virtual void OnClickUp(CPoint PointCellRelative);
    virtual void OnClickDown(CPoint PointCellRelative);

protected:
    struct ButtonInfo
    {
        CString m_name;
        CRect m_rect;
    };
    std::vector<ButtonInfo> m_buttons;
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

    long GetButtonIndex(CPoint PointCellRelative);
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

protected:
    BOOL    m_bEnabled;
    long    m_downedButtonIndex;
    CRect   m_Rect;
};
