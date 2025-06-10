#include "pch.h"
#include "Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PROGRAMPATH_ROOT										     _T("C:\\SW\\")
#define PROGRAMPATH_SYSTEM							PROGRAMPATH_ROOT _T("System\\")
#define PROGRAMPATH_SYSTEMFILE						PROGRAMPATH_SYSTEM _T("config.ini")
#define PROGRAMPATH_LOG								PROGRAMPATH_ROOT _T("Log\\")

LPCTSTR Path::GetRootFolderPath() { return PROGRAMPATH_ROOT; }
LPCTSTR Path::GetSystemFolderPath() { return PROGRAMPATH_SYSTEM; }
LPCTSTR Path::GetLogFolderPath() { return PROGRAMPATH_LOG; }
LPCTSTR Path::GetSystemFilePath() { return PROGRAMPATH_SYSTEMFILE; }

