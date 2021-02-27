

#include "pch.h"

#define MD_Throw_Handle(szError) throw LXY::m_HandleException(__FILE__,__LINE__,#szError, GetLastError())

HANDLE hThreadInfiniteSunshine = NULL;
HANDLE hEvent = NULL;

//������������
void m_FixTheSunLight(DWORD dwSunLightNumber)
{
	
	__try
	{
		PDWORD pSunLight = PDWORD(*(PDWORD)(*(PDWORD)SUN_BASE + SUN_OFFSET1) + SUN_OFFSET2);
		*pSunLight = dwSunLightNumber;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}

}

//���������߳�
static unsigned ThreadProcInfiniteSunshine(void* pvParam)
{
	while (true)
	{
		m_FixTheSunLight(9999);
		DWORD dwRet =  WaitForSingleObject(hEvent, 0x30);
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
//������������
void m_InfiniteSunshine()
{
	if (hEvent == NULL)
	{
		if ((hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
		{
			MD_Throw_Handle(m_InfiniteSunshine::lxy_BEGINTHREADEX);
		}//�����ȴ��������ڽ����߳�
		if ((hThreadInfiniteSunshine = lxy_BEGINTHREADEX(NULL, 0, ThreadProcInfiniteSunshine, 0, 0, NULL)) == NULL)
		{
			MD_Throw_Handle(m_InfiniteSunshine::lxy_BEGINTHREADEX);
		}//������������������߳�
	}//�Ѿ���һ���߳��� �Ͳ���ȥ��
}
//������������
void m_ExitInfiniteSunshine()
{
	if (hEvent)
	{
		SetEvent(hEvent);
		if (hThreadInfiniteSunshine)
		{
			WaitForSingleObject(hThreadInfiniteSunshine, INFINITE);
			CloseHandle(hThreadInfiniteSunshine);
		}
		CloseHandle(hEvent);
		hThreadInfiniteSunshine = NULL;
		hEvent = NULL;
	}
}
