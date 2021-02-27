#include "pch.h"

#define WNDNAME TEXT("ZhiWuDaZhanJiangShiHelper")
void RegisterWindow(HMODULE hModule)
{
	//1  设计一个窗口类
	WNDCLASS wnd;
	wnd.style = CS_VREDRAW | CS_HREDRAW;//风格
	wnd.lpfnWndProc = WndProc;//窗口回调函数指针.
	wnd.cbClsExtra = NULL;
	wnd.cbWndExtra = NULL;
	wnd.hInstance = hModule;
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = WNDNAME;
	//2  注册窗口类
	RegisterClass(&wnd);
	//3  创建窗口
	HWND hWnd = CreateWindow(
		WNDNAME,  //窗口类名
		WNDNAME,//窗口名
		WS_OVERLAPPEDWINDOW,//窗口风格
		10, 10, 500, 300, //窗口位置
		NULL,             //父窗口句柄
		NULL,             //菜单句柄
		hModule,        //实例句柄
		NULL              //传递WM_CREATE消息时的附加参数
	);
	//4  更新显示窗口
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	//5  消息循环（消息泵）
	MSG  msg = {};
	//   5.1获取消息
	while (GetMessage(&msg, 0, 0, 0))
	{
		//   5.2翻译消息
		TranslateMessage(&msg);
		//   5.3转发到消息回调函数
		DispatchMessage(&msg);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		//销毁窗口并退出线程
		PostQuitMessage(0);
		break;
	case UDM_AMOUNT_OF_SUNLIGHT: //设置阳光
	{
		m_FixTheSunLight((DWORD)wParam);
	}break;
	case UDM_INFINITE_SUNSHINE: //无限阳光
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_InfiniteSunshine();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("初始化无限阳光错误");
				m_ExitInfiniteSunshine();
			}
		}
		else if ((bool)wParam == false)
		{
			m_ExitInfiniteSunshine();
		}

	}break;
	case UDM_NO_COOLING_TIME:	//无冷却时间
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_NoCoolingTime();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("初始化无冷却时间错误");
				m_ExitNoCoolingTime();
			}
		}
		else if ((bool)wParam == false)
		{
			m_ExitNoCoolingTime();
		}
	}break;
	case UDM_ZOMBIE_TAKEDOWN:	//秒杀僵尸
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_KillZombiesWithOneHit();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("初始化秒杀僵尸错误");
			}
		}
		else if ((bool)wParam == false)
		{	
			m_ExitKillZombiesWithOneHit();
		}
	}break;
	case UDM_BULLET_SPLIT:		//子弹分身
	{
		if ((bool)wParam == true)
		{
			m_ZiDanFengSheng();
		}
		else if ((bool)wParam == false)
		{
			m_ExitZiDanFengSheng();
		}
	}break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}