#pragma once
#include "PreDefined.h"
#include <memory>

class __BASE_CLASS__ IniGetApp
{
public:
	IniGetApp(LPCTSTR filePath, LPCTSTR appName);

	CString GetValue(LPCTSTR keyName, LPCTSTR defaultValue = _T(""));
	void SetValue(LPCTSTR keyName, LPCTSTR value);
	void Link(bool save, LPCTSTR keyName, CString& io_value, LPCTSTR defaultValue = _T(""));
	void Link(bool save, LPCTSTR keyName, bool& io_value, bool defaultValue = false);
	void Link(bool save, LPCTSTR keyName, int32_t& io_value, int32_t defaultValue = 0);
	void Link(bool save, LPCTSTR keyName, long& io_value, long defaultValue = 0);
	void Link(bool save, LPCTSTR keyName, float& io_value, float defaultValue = 0);
private:
	CString m_filePath;
	CString m_appName;
};

class __BASE_CLASS__ IniFile
{
public:
	IniFile(LPCTSTR filePath);
	std::shared_ptr<IniGetApp> GetApp(LPCTSTR appName);

private:
	CString m_filePath;
};
