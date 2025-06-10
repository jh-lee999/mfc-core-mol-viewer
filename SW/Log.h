#pragma once

#define UM_APPENDED_LOG	(WM_USER + 100)

enum LogType
{
	System,
	Debug,
	END,
};

LPCTSTR EnumToText(LogType type);

#include <queue>
#include <map>
#include <set>
#include <thread>
#include "../Base/Path.h"
#include "../Base/Utility.h"

class Log
{
public:
	Log();
	~Log();

	void SetWnd(HWND wnd);
	void Write(LogType type, LPCTSTR fmt, ...);
	void Pop(std::queue<std::pair<LogType, CString>>& o_logList);

	void WriteThread_Pause();	// 일시정지
	void WriteThread_Resume();	// 재개

private:
	HWND m_targetHwnd;
	std::thread m_thread;
	CCriticalSection m_cs;
	std::queue<std::pair<LogType, CString>> m_logListForPop;
	std::map<long, std::map<LogType, std::queue<CString>>> m_logList;

	std::set<long> m_logDays;

	CEvent m_eventWrite;
	CEvent m_eventEnd;

	void ThreadProc();
	bool DeleteFolder(LPCTSTR folderPath);
};

extern Log g_log;
