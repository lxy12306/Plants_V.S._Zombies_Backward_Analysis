// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
HMODULE g_hModule = NULL;


unsigned ThreadProc(void* pvParam)//执行初始化操作
{
    LXY::MyInlineHook32 g_Mih(0x004351BB, 9, m_fJiangShiMiaoSha);
    g_Mih.e_InlineHook1();

    LXY::MyInlineHook32 g_Mih2(ZI_DAN_FENG_SHENG, 5, m_fZiDanFengSheng);
    g_Mih2.e_InlineHook2();

	RegisterWindow(g_hModule);

    g_Mih.e_ExitInlineHook();
    g_Mih2.e_ExitInlineHook();

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
		g_hModule = hModule;
        lxy_BEGINTHREADEX(NULL, 0, ThreadProc, 0, 0, 0);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        //SendMessage(HWND_BROADCAST, WM_CLOSE, 0, 0);
        break;
    }
    return TRUE;
}

