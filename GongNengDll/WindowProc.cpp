#include "pch.h"

#define WNDNAME TEXT("ZhiWuDaZhanJiangShiHelper")
void RegisterWindow(HMODULE hModule)
{
	//1  ���һ��������
	WNDCLASS wnd;
	wnd.style = CS_VREDRAW | CS_HREDRAW;//���
	wnd.lpfnWndProc = WndProc;//���ڻص�����ָ��.
	wnd.cbClsExtra = NULL;
	wnd.cbWndExtra = NULL;
	wnd.hInstance = hModule;
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = WNDNAME;
	//2  ע�ᴰ����
	RegisterClass(&wnd);
	//3  ��������
	HWND hWnd = CreateWindow(
		WNDNAME,  //��������
		WNDNAME,//������
		WS_OVERLAPPEDWINDOW,//���ڷ��
		10, 10, 500, 300, //����λ��
		NULL,             //�����ھ��
		NULL,             //�˵����
		hModule,        //ʵ�����
		NULL              //����WM_CREATE��Ϣʱ�ĸ��Ӳ���
	);
	//4  ������ʾ����
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	//5  ��Ϣѭ������Ϣ�ã�
	MSG  msg = {};
	//   5.1��ȡ��Ϣ
	while (GetMessage(&msg, 0, 0, 0))
	{
		//   5.2������Ϣ
		TranslateMessage(&msg);
		//   5.3ת������Ϣ�ص�����
		DispatchMessage(&msg);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		//���ٴ��ڲ��˳��߳�
		PostQuitMessage(0);
		break;
	case UDM_AMOUNT_OF_SUNLIGHT: //��������
	{
		m_FixTheSunLight((DWORD)wParam);
	}break;
	case UDM_INFINITE_SUNSHINE: //��������
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_InfiniteSunshine();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("��ʼ�������������");
				m_ExitInfiniteSunshine();
			}
		}
		else if ((bool)wParam == false)
		{
			m_ExitInfiniteSunshine();
		}

	}break;
	case UDM_NO_COOLING_TIME:	//����ȴʱ��
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_NoCoolingTime();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("��ʼ������ȴʱ�����");
				m_ExitNoCoolingTime();
			}
		}
		else if ((bool)wParam == false)
		{
			m_ExitNoCoolingTime();
		}
	}break;
	case UDM_ZOMBIE_TAKEDOWN:	//��ɱ��ʬ
	{
		if ((bool)wParam == true)
		{
			try
			{
				m_KillZombiesWithOneHit();
			}
			catch (const LXY::m_WindowsException&)
			{
				LXY::m_PrintMessageBox("��ʼ����ɱ��ʬ����");
			}
		}
		else if ((bool)wParam == false)
		{	
			m_ExitKillZombiesWithOneHit();
		}
	}break;
	case UDM_BULLET_SPLIT:		//�ӵ�����
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