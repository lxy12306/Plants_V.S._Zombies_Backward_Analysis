#include "F:\Code\object\WinUserHandle\MyWindows.h"
#include "F:/Code/object/Exception/MyExceptionWindows.h"
#include "Messages.h"

#define MD_Throw_Handle(szError) throw LXY::m_HandleException(__FILE__,__LINE__,#szError, GetLastError())

extern HWND g_hWndDll;

void m_FindMyWindow()
{
#define MAXSIZE 100
	int i = MAXSIZE;
	while (i--)
	{
		g_hWndDll = FindWindow(TEXT("ZhiWuDaZhanJiangShiHelper")/*窗口类型*/, TEXT("ZhiWuDaZhanJiangShiHelper")/*窗口名*/);
		if (g_hWndDll != NULL)
			return;
		Sleep(10);
	}
	if (g_hWndDll == NULL)
		MD_Throw_Handle(FindWindow);
}

