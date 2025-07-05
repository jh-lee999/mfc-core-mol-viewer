#pragma once
#include "PreDefined.h"
#include <vector>

class __BASE_CLASS__ Utility
{
public:
	static CString	ToString(const SYSTEMTIME& value);
	static CString	ToString(const int32_t& value);
	static CString	ToString(const long& value);
	static CString	ToString(const float& value);
	static CString	FormatString(LPCTSTR fmt, ...);
	static SYSTEMTIME IncreaseTime(const SYSTEMTIME& base, int days, int hours, int mins, int secs);
	static SYSTEMTIME ToDateTime(LPCTSTR text);
	static int		GetControlWidth(CWnd& wnd);
	static void		ControlAlignRight(long cx, CWnd& wnd);
	static CString	GetSelectedComboBoxText(CComboBox& control);
	static void		SetSelectedComboBoxText(CComboBox& control, LPCTSTR text, int32_t index_if_not_found = -1);
public:
	static CString	CreateGUID();
	static CString	GetLocale();
	static void		SetLocale(LPCTSTR locale);
	static long		GetMonitorCount();
	static CRect	GetMonitorRegion(long monitorIndex);
	static bool		IsCompatableVersion(LPCTSTR version);
	static bool		ReadResource(LPCTSTR type, UINT resourceID, std::vector<BYTE>& o_binaryData);
	static bool		IsFileExist(LPCTSTR file_path);
};
