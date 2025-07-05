#include "pch.h"
#include "IniFile.h"
#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IniGetApp::IniGetApp(LPCTSTR filePath, LPCTSTR appName)
	: m_filePath(filePath)
	, m_appName(appName)
{
}

CString IniGetApp::GetValue(LPCTSTR keyName, LPCTSTR defaultValue)
{
	TCHAR text[MAX_PATH];
	GetPrivateProfileString(m_appName, keyName, defaultValue, text, MAX_PATH, m_filePath);

	return text;
}

void IniGetApp::SetValue(LPCTSTR keyName, LPCTSTR value)
{
	WritePrivateProfileString(m_appName, keyName, value, m_filePath);
}

void IniGetApp::Link(bool save, LPCTSTR keyName, CString& io_value, LPCTSTR defaultValue)
{
	if (save)
	{
		SetValue(keyName, io_value);
	}
	else
	{
		io_value = GetValue(keyName, defaultValue);
	}
}

void IniGetApp::Link(bool save, LPCTSTR keyName, bool& io_value, bool defaultValue)
{
	if (save)
	{
		SetValue(keyName, io_value ? _T("1") : _T("0"));
	}
	else
	{
		io_value = _ttoi(GetValue(keyName, Utility::FormatString(_T("%d"), defaultValue ? 1 : 0))) ? true : false;
	}
}

void IniGetApp::Link(bool save, LPCTSTR keyName, int32_t& io_value, int32_t defaultValue)
{
	if (save)
	{
		SetValue(keyName, Utility::ToString(io_value));
	}
	else
	{
		io_value = _ttoi(GetValue(keyName, Utility::ToString(defaultValue)));
	}
}

void IniGetApp::Link(bool save, LPCTSTR keyName, long& io_value, long defaultValue)
{
	if (save)
	{
		SetValue(keyName, Utility::ToString(io_value));
	}
	else
	{
		io_value = _ttoi(GetValue(keyName, Utility::ToString(defaultValue)));
	}
}

void IniGetApp::Link(bool save, LPCTSTR keyName, float& io_value, float defaultValue)
{
	if (save)
	{
		SetValue(keyName, Utility::ToString(io_value));
	}
	else
	{
		io_value = (float)_ttof(GetValue(keyName, Utility::ToString(defaultValue)));
	}
}

IniFile::IniFile(LPCTSTR filePath)
	: m_filePath(filePath)
{
}

std::shared_ptr<IniGetApp> IniFile::GetApp(LPCTSTR appName)
{
	return std::make_shared<IniGetApp>(m_filePath, appName);
}
