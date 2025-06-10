#include "pch.h"
#include "Predefined.h"
#include "SWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PostCommand(PostCommandType type, std::shared_ptr<PostObj> obj)
{
	g_postProc->Post((long)type, obj);	//StyleDialog의 함수 사용
}
