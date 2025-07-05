// CustomControl.h : main header file for the CustomControl DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCustomControlApp
// See CustomControl.cpp for the implementation of this class
//

class CCustomControlApp : public CWinApp
{
public:
	CCustomControlApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
