#include "pch.h"
#include "Utility.h"
#include "Path.h"
#include "../SW/VersionDef.h"

#include <locale.h>

#pragma comment(lib, "Rpcrt4.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString	Utility::ToString(const SYSTEMTIME& value)
{
	CString text;
	text.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		value.wYear, value.wMonth, value.wDay,
		value.wHour, value.wMinute, value.wSecond);

	return text;
}

CString Utility::ToString(const int32_t& value)
{
	CString text;
	text.Format(_T("%d"), value);

	return text;
}

CString Utility::ToString(const long& value)
{
	CString text;
	text.Format(_T("%d"), value);

	return text;
}

CString Utility::ToString(const float& value)
{
	CString text;
	text.Format(_T("%f"), value);

	return text;
}

CString Utility::FormatString(LPCTSTR fmt, ...)
{
	CString text;

	va_list argptr;

	va_start(argptr, fmt);
	text.FormatV(fmt, argptr);
	va_end(argptr);

	return text;
}

SYSTEMTIME Utility::IncreaseTime(const SYSTEMTIME& base, int days, int hours, int mins, int secs)
{
	CTime tm = CTime(base) + CTimeSpan(days, hours, mins, secs);
	SYSTEMTIME retValue;
	tm.GetAsSystemTime(retValue);

	return retValue;
}

SYSTEMTIME Utility::ToDateTime(LPCTSTR text)
{
	SYSTEMTIME systime;

	memset(&systime, 0, sizeof(systime));

	// yyyy-MM-dd hh:mm:ss
	_stscanf_s(text, _T("%hd-%hd-%hd %hd:%hd:%hd"),
		&systime.wYear,
		&systime.wMonth,
		&systime.wDay,
		&systime.wHour,
		&systime.wMinute,
		&systime.wSecond);

	return systime;
}

int Utility::GetControlWidth(CWnd& wnd)
{
	CRect controlRect;
	wnd.GetClientRect(controlRect);

	return controlRect.Width();
}

void Utility::ControlAlignRight(long cx, CWnd& wnd)
{
	CRect controlRect;
	wnd.GetWindowRect(controlRect);
	wnd.GetParent()->ScreenToClient(controlRect);

	wnd.MoveWindow(cx - controlRect.Width(), controlRect.top, controlRect.Width(), controlRect.Height());
}

CString	Utility::GetSelectedComboBoxText(CComboBox& control)
{
	auto selected_index = control.GetCurSel();
	if (selected_index < 0) return L"";

	CString text;
	control.GetLBText(selected_index, text);

	return text;
}

void Utility::SetSelectedComboBoxText(CComboBox& control, LPCTSTR text, int32_t index_if_not_found)
{
	for (int32_t index = 0; index < control.GetCount(); index++)
	{
		CString option_text;
		control.GetLBText(index, option_text);
		if (option_text == text)
		{
			control.SetCurSel(index);
			return;
		}
	}

	control.SetCurSel(index_if_not_found);
}



CString Utility::GetLocale()
{
	char saved_locale[MAX_PATH];
	strcpy_s(saved_locale, setlocale(LC_ALL, nullptr));

	return CString(saved_locale);
}

void Utility::SetLocale(LPCTSTR locale)
{
	setlocale(LC_ALL, (const char*)CStringA(locale));
}

CString Utility::CreateGUID()
{
	CString guid;

	UUID uuid;

	if (::UuidCreate(&uuid) == RPC_S_OK)
	{
		TCHAR* sz = nullptr;

		if (::UuidToString(&uuid, (RPC_WSTR*)&sz) == RPC_S_OK)
		{
			guid = sz;

			::RpcStringFree((RPC_WSTR*)&sz);
		}
	}

	return guid;
}



BOOL CALLBACK MonitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT lprect, LPARAM lparam)
{
	auto& infos = *((std::vector<MONITORINFO> *)lparam);
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hmonitor, &mi);
	infos.push_back(mi);

	return TRUE;
}

std::vector<MONITORINFO> GetMonitorInfos()
{
	std::vector<MONITORINFO> infos;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&infos);

	return infos;
}

long Utility::GetMonitorCount()
{
	auto infos = GetMonitorInfos();
	return (long)infos.size();
}

CRect Utility::GetMonitorRegion(long monitorIndex)
{
	auto infos = GetMonitorInfos();
	if (infos.size() == 0)
	{
		return CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}

	if (monitorIndex < 0 || monitorIndex >= (long)infos.size())
	{
		monitorIndex = 0;
	}

	return infos[monitorIndex].rcMonitor;
}

bool Utility::IsCompatableVersion(LPCTSTR version)
{
	CString versionText(version);
	long dot1 = versionText.Find(_T('.'));
	if (dot1 < 0) return false;

	long dot2 = versionText.Find(_T('.'), dot1 + 1);
	if (dot2 < 0) return false;

	auto versionHead = versionText.Left(dot2);
	auto hostVersionHead = CString(SW_MACRO_VERSION_STR(SW_MAJOR_VERSION) "." SW_MACRO_VERSION_STR(SW_MINOR_VERSION));

	// Major와 Minor 버전이 같은 버전만 호환된다
	return versionHead == hostVersionHead;
}

bool Utility::ReadResource(LPCTSTR type, UINT resourceID, std::vector<BYTE>& o_binaryData)
{
	HMODULE hModule = GetModuleHandle(NULL);
	if (auto hRes = FindResource(hModule, MAKEINTRESOURCE(resourceID), type))
	{
		if (auto hglob = LoadResource(hModule, hRes))
		{
			void* buf = LockResource(hglob);
			UINT finelen = SizeofResource(hModule, hRes);

			o_binaryData.resize(finelen);
			if (finelen >= 0)
			{
				memcpy_s(&o_binaryData[0], finelen, buf, finelen);
			}
			return true;
		}
	}

	return false;
}

bool Utility::IsFileExist(LPCTSTR file_path)
{
	return ::PathFileExists(file_path) ? TRUE : FALSE;
}

