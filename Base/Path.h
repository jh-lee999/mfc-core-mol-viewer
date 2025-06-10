#pragma once
#include "PreDefined.h"

class __BASE_CLASS__ Path
{
public:
	static LPCTSTR GetRootFolderPath();
	static LPCTSTR GetSystemFolderPath();
	static LPCTSTR GetLogFolderPath();
	static LPCTSTR GetSystemFilePath();
};
