
#include "pch.h"

#define MD_Throw_Windows(szError) throw LXY::m_WindowsException(__FILE__,__LINE__,#szError, GetLastError())
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// ��ȴʱ�����
/// </summary>

#define COOLINGTIME_ONE_STRUCT 0x50	//ÿ��������ȴʱ����
#define COOLINGTIME_COUNT 0x24 //�����ڱ任��ʵ����ȴʱ��
#define COOLINGTIME_FULL 0x28	//����ȴʱ��

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
	DWORD dwFullCoolingTime = 0; //����ȴʱ��
	for (int i = 0; i < 6; ++i)
	{
		__try
		{
			dwFullCoolingTime = *PDWORD((DWORD)pGeZiStruct + COOLINGTIME_FULL);
			*PDWORD((DWORD)pGeZiStruct + COOLINGTIME_COUNT) = dwFullCoolingTime;

			pGeZiStruct = PDWORD((DWORD)pGeZiStruct + COOLINGTIME_ONE_STRUCT); //��һ������
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return;
		}
	}
}

//����ȴʱ��
static unsigned ThreadProcNoCoolingTime(void* pvParam)
{
	while (true)
	{
		m_FixCoolingTime();
		DWORD dwRet = WaitForSingleObject(hEventToStop, 0x30);
		if (dwRet == WAIT_OBJECT_0) //�˳�
		{
			break;
		}
		else if (dwRet == WAIT_TIMEOUT) //û�н��� �����
		{
			continue;
		}
	}
	return 0;
}
//��������ȴʱ��
void m_NoCoolingTime()
{
	if (hEventToStop == NULL)
	{
		if ((hEventToStop = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
		{
			MD_Throw_Windows(m_NoCoolingTime::lxy_BEGINTHREADEX);
		}//�����ȴ��������ڽ����߳�
		if ((hThreadNoCoolingTime = lxy_BEGINTHREADEX(NULL, 0, ThreadProcNoCoolingTime, 0, 0, NULL)) == NULL)
		{
			MD_Throw_Windows(m_NoCoolingTime::lxy_BEGINTHREADEX);
		}//������������������߳�
	}//�Ѿ���һ���߳��� �Ͳ���ȥ��
}
//��������ȴʱ��
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

//ֲ���ӵ�����
static DWORD g_dwCall = ZHIWU_ZIDAN_FASHE_CALL_BASE; //�����ӵ�call
static bool g_bIsZiDanFengSheng = false;//�����Ƿ�ʼ

#define ZHIWU_YPOSITON_FIRST 0x50 //��һ��Y����
#define ZHIWU_YPOSITON_OFFSET 0x64	//Y����֮���ƫ��
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

	dwPositionYOld = *PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET);//�����ֲ��֮ǰ��Y����
	for (int i = 0; i < 5; i++, dwPositionYNew += ZHIWU_YPOSITON_OFFSET)
	{
		if (dwPositionYNew != dwPositionYOld)//ͬ�в��ٷ����ӵ���
		{
			*PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET) = dwPositionYNew;//�����µķ����ӵ�����

			__asm
			{
				push 0
				push i
				push 0
				push pdwZhiWu
				call g_dwCall
			}//�����ӵ�
		}
	}
	*PDWORD((DWORD)pdwZhiWu + JIANGSHI_YPOSITON_OFFSET) = dwPositionYOld; //��ԭֲ������
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
