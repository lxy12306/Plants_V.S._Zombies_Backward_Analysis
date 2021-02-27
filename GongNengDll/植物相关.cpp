
#include "pch.h"

#define MD_Throw_Windows(szError) throw LXY::m_WindowsException(__FILE__,__LINE__,#szError, GetLastError())
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 冷却时间相关
/// </summary>

#define COOLINGTIME_ONE_STRUCT 0x50	//每个格子冷却时间间隔
#define COOLINGTIME_COUNT 0x24 //不断在变换的实际冷却时间
#define COOLINGTIME_FULL 0x28	//满冷却时间

HANDLE hThreadNoCoolingTime = NULL;
HANDLE hEventToStop = NULL;
bool  g_bOk = true;


static void m_FixCoolingTime(void)
{

	PDWORD pGeZiStruct;
	__try
	{
		pGeZiStruct = PDWORD(*PDWORD(*(PDWORD)(*(PDWORD)IMPORTANT1_BASE + IMPORTANT1_OFFSET1) + COOLINGTIME_OFFSET2) + COOLINGTIME_OFFSET3);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	DWORD dwFullCoolingTime = 0; //满冷却时间
	for (int i = 0; i < 6; ++i)
	{
		__try
		{
			dwFullCoolingTime = *PDWORD((DWORD)pGeZiStruct + COOLINGTIME_FULL);
			*PDWORD((DWORD)pGeZiStruct + COOLINGTIME_COUNT) = dwFullCoolingTime;

			pGeZiStruct = PDWORD((DWORD)pGeZiStruct + COOLINGTIME_ONE_STRUCT); //下一个格子
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return;
		}
	}
}

//无冷却时间
static unsigned ThreadProcNoCoolingTime(void* pvParam)
{
	while (true)
	{
		m_FixCoolingTime();
		DWORD dwRet = WaitForSingleObject(hEventToStop, 0x30);
		if (dwRet == WAIT_OBJECT_0) //退出
		{
			break;
		}
		else if (dwRet == WAIT_TIMEOUT) //没有结束 则继续
		{
			continue;
		}
	}
	return 0;
}
//启动无冷却时间
void m_NoCoolingTime()
{
	if (hEventToStop == NULL)
	{
		if ((hEventToStop = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
		{
			MD_Throw_Windows(m_NoCoolingTime::lxy_BEGINTHREADEX);
		}//创建等待对象用于结束线程
		if ((hThreadNoCoolingTime = lxy_BEGINTHREADEX(NULL, 0, ThreadProcNoCoolingTime, 0, 0, NULL)) == NULL)
		{
			MD_Throw_Windows(m_NoCoolingTime::lxy_BEGINTHREADEX);
		}//启动设置无限阳光的线程
	}//已经有一个线程了 就不会去了
}
//结束无冷却时间
void m_ExitNoCoolingTime()
{
	if (hEventToStop)
	{
		SetEvent(hEventToStop);
		if (hThreadNoCoolingTime)
		{
			WaitForSingleObject(hThreadNoCoolingTime, INFINITE);
			CloseHandle(hThreadNoCoolingTime);
		}
		CloseHandle(hEventToStop);
		hThreadNoCoolingTime = NULL;
		hEventToStop = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

//植物子弹分身
static DWORD g_dwCall = ZHIWU_ZIDAN_FASHE_CALL_BASE; //发射子弹call
static bool g_bIsZiDanFengSheng = false;//控制是否开始

#define ZHIWU_YPOSITON_FIRST 0x50 //第一个Y坐标
#define ZHIWU_YPOSITON_OFFSET 0x64	//Y坐标之间的偏移
void __declspec(naked) m_fZiDanFengSheng(void)
{
	__asm
	{
		push ebp;
		mov ebp, esp;
		sub esp, 0x100;
	}
	PDWORD pdwZhiWu;
	__asm
	{
		mov pdwZhiWu, ebx;
	}
	if (g_bIsZiDanFengSheng == false)
	{
		goto exit;
	}

	DWORD dwPositionYOld;
	DWORD dwPositionYNew;
	__asm
	{
		mov dword ptr dwPositionYNew, ZHIWU_YPOSITON_FIRST;
	}

	dwPositionYOld = *PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET);//保存该植物之前的Y坐标
	for (int i = 0; i < 5; i++, dwPositionYNew += ZHIWU_YPOSITON_OFFSET)
	{
		if (dwPositionYNew != dwPositionYOld)//同行不再发射子弹了
		{
			*PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET) = dwPositionYNew;//更新新的发射子弹坐标

			__asm
			{
				push 0
				push i
				push 0
				push pdwZhiWu
				call g_dwCall
			}//发射子弹
		}
	}
	*PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET) = dwPositionYOld; //还原植物坐标
exit:
	__asm
	{
		leave;
		ret;
	}
}
void m_ZiDanFengSheng()
{
	g_bIsZiDanFengSheng = true;
}

void m_ExitZiDanFengSheng()
{
	g_bIsZiDanFengSheng = false;
}
