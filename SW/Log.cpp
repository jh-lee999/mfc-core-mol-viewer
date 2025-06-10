#include "pch.h"
#include "Log.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR EnumToText(LogType type)
{
	//항목이 더 필요하면 추가
	switch (type)
	{
	case LogType::System:			return _T("System");
	case LogType::Debug:			return _T("Debug");
	}

	return _T("");
}

Log g_log;

Log::Log()
	: m_targetHwnd(NULL)
{
	// Log 날짜폴더명들을 수집한다
	CFileFind ff;
	auto foundFile = ff.FindFile(Utility::FormatString(_T("%s*.*"), Path::GetLogFolderPath()));

	while (foundFile)
	{
		foundFile = ff.FindNextFile();
		if (!ff.IsDirectory()) continue;
		if (ff.IsDots()) continue;

		auto dayID = _ttoi(ff.GetFileName());
		if (dayID < 10000000) continue; // 날짜 폴더가 아님
		m_logDays.insert(dayID);
	}

	ff.Close();

	WriteThread_Resume();
}

Log::~Log()
{
	WriteThread_Pause();
}

void Log::SetWnd(HWND wnd)
{
	m_cs.Lock();

	m_targetHwnd = wnd;
	if (m_targetHwnd != NULL)
	{
		PostMessage(m_targetHwnd, UM_APPENDED_LOG, 0, 0);
	}

	m_cs.Unlock();
}

void Log::Write(LogType type, LPCTSTR fmt, ...)
{
	CString text;

	va_list argptr;

	va_start(argptr, fmt);
	text.FormatV(fmt, argptr);
	va_end(argptr);

	m_cs.Lock();

	SYSTEMTIME tm;
	GetLocalTime(&tm);

	CString logText;
	logText.Format(_T("%02d:%02d:%02d:%03ld %s"), tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds, (LPCTSTR)text);
	long fileID = tm.wYear * 1000000L + tm.wMonth * 10000L + tm.wDay * 100L + tm.wHour;

	m_logList[fileID][type].emplace(logText);
	m_logListForPop.emplace(type, logText);

	if (m_targetHwnd != NULL)
	{
		PostMessage(m_targetHwnd, UM_APPENDED_LOG, 0, 0);
	}

	m_cs.Unlock();
	m_eventWrite.SetEvent();
}

void Log::Pop(std::queue<std::pair<LogType, CString>>& o_logList)
{
	m_cs.Lock();
	o_logList = m_logListForPop;
	m_logListForPop = std::queue<std::pair<LogType, CString>>();
	m_cs.Unlock();
}

void Log::WriteThread_Pause()
{
	if (m_thread.joinable())
	{
		m_eventEnd.SetEvent();
		m_thread.join();
	}
}

void Log::WriteThread_Resume()
{
	if (m_thread.joinable()) return;	// 이미 Thread 동작중

	m_eventEnd.ResetEvent();

	// Log Thread 동작
	m_thread = std::thread([](Log* pThis) { pThis->ThreadProc(); }, this);
}

void Log::ThreadProc()
{

	HANDLE handles[] = { m_eventWrite, m_eventEnd };
	while (true)
	{
		auto select = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
		if (select == WAIT_OBJECT_0 + 1) break;

		m_cs.Lock();
		auto logList = m_logList;
		m_logList.clear();
		m_cs.Unlock();

		for (auto& itDayLog : logList)
		{
			auto fileID = itDayLog.first;
			auto dayID = fileID / 100;

			CString logDayDirectory, logHourDirectory;
			logDayDirectory.Format(_T("%s%08d\\"), Path::GetLogFolderPath(), dayID);
			CreateDirectory(logDayDirectory, nullptr);

			CString checkFolder = logDayDirectory;
			checkFolder.MakeUpper();
			m_logDays.insert(dayID);

			logHourDirectory.Format(_T("%s%02d\\"), (LPCTSTR)logDayDirectory, fileID % 100);
			CreateDirectory(logHourDirectory, nullptr);

			for (auto& itTypeLog : itDayLog.second)
			{
				auto type = itTypeLog.first;
				auto& queue = itTypeLog.second;

				CString filePath;
				filePath.Format(_T("%s%s.txt"), (LPCTSTR)logHourDirectory, EnumToText(type));

				CStdioFile file;
				if (file.Open(filePath, CStdioFile::modeCreate | CStdioFile::typeText | CStdioFile::modeReadWrite | CStdioFile::modeNoTruncate))
				{
					file.SeekToEnd();

					while (!queue.empty())
					{
						CString text = queue.front();
						file.WriteString(text + _T("\n"));
						queue.pop();
					}

					file.Close();
				}
			}

			CTime today = CTime(dayID / 10000, (dayID / 100) % 100, dayID % 100, 0, 0, 0);

			while (m_logDays.size() > 0)
			{
				auto checkDayID = *m_logDays.begin();
				CTime checkDay = CTime(checkDayID / 10000, (checkDayID / 100) % 100, checkDayID % 100, 0, 0, 0);

				if (today - checkDay >= CTimeSpan(max(1, 500), 0, 0, 0))
				{
					CString deleteFolder;
					deleteFolder.Format(_T("%s%08d\\"), Path::GetLogFolderPath(), checkDayID);
					DeleteFolder(deleteFolder);

					m_logDays.erase(checkDayID);
					continue;
				}
				break;
			}
		}

		// 1초에 한번씩 몰아서 하자
		Sleep(1000);
	}
}

bool Log::DeleteFolder(LPCTSTR folderPath)
{
	CFileFind ff;
	auto foundFile = ff.FindFile(Utility::FormatString(_T("%s\\*.*"), folderPath));

	while (foundFile)
	{
		foundFile = ff.FindNextFile();
		if (!ff.IsDirectory())
		{
			DeleteFile(ff.GetFilePath());
			continue;
		}

		if (ff.IsDots()) continue;

		DeleteFolder(ff.GetFilePath());
	}

	ff.Close();

	return RemoveDirectory(folderPath);
}
