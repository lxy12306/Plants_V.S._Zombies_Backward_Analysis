#include "pch.h"
#include "Hook.h"
#include "F:\Code\object\ProcessAndHwndAndThread\MyThread.h"
#include "F:/Code/object/Exception/MyExceptionWindows.h"

#define MD_Throw_Windows(szError) throw LXY::m_WindowsException(__FILE__,__LINE__,#szError, GetLastError())

using namespace std;
namespace LXY
{

	void MyInlineHook32::m_StopAllThreadExpect(unsigned dwThreadId)
	{
		vector<LXY_PROCESS_THREADINFO> vlst; 
		
		MyThreadInfo mti;
		mti.e_QueryThreadInfo_By_Pid(dwPid, vlst); //�оٳ��߳���Ϣ

		vechThreads.clear();
		for (auto value : vlst)
		{
			if (value.dwTid == dwThreadId)
				continue; //�Լ�����̲߳���ͣ
			auto hThread = ::OpenThread(THREAD_ALL_ACCESS,false,value.dwTid);
			if (hThread == NULL) //�Ѿ����о��߳��� ���Դ��߳̾������Ȩ�޵� ���Գ���Ļ������߳���ʧ��
				continue;
			vechThreads.emplace_back(hThread);
		}
		for (auto value : vechThreads)
		{
			SuspendThread(value);
		}
	}

	void MyInlineHook32::m_InlineHook1() //ռ�õĵط����ȵ���
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);
		m_lpvAddress = VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_lpvAddress == NULL)
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualAllocEx);
		}
		//HOOK��ת�����ڴ�

		m_FixHook1();
		BYTE* pbBuffer = new BYTE[m_dwHookSize];
		memset(pbBuffer, 0x90, m_dwHookSize);
		memcpy_s(pbBuffer, m_dwHookSize, abBuffer, 5); //HOOK�������ֽ�ʹ��nop���

		m_pOldBuffer = new BYTE[m_dwHookSize];
		ReadProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//����ԭ����HOOK�Ķ���
		if (!VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, PAGE_EXECUTE_READWRITE, &m_dwOldProtect))
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualProtectEx);
		}
		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, pbBuffer, m_dwHookSize, NULL);//inlineHook

		WriteProcessMemory(hProcess, m_lpvAddress, m_pOldBuffer, m_dwHookSize, NULL);//ԭ���ĺ���
		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress+ m_dwHookSize), abBufferCall, 0x100, NULL);

		CloseHandle(hProcess);
		delete[]	pbBuffer;
	}
	void MyInlineHook32::m_InlineHook2() //ռ�õĵط����ȵ���
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);
		m_lpvAddress = VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_lpvAddress == NULL)
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualAllocEx);
		}
		//HOOK��ת�����ڴ�

		m_FixHook2();
		BYTE* pbBuffer = new BYTE[m_dwHookSize];
		memset(pbBuffer, 0x90, m_dwHookSize);
		memcpy_s(pbBuffer, m_dwHookSize, abBuffer, 5); //HOOK�������ֽ�ʹ��nop���

		m_pOldBuffer = new BYTE[m_dwHookSize];
		ReadProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//����ԭ����HOOK�Ķ���
		if (!VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, PAGE_EXECUTE_READWRITE, &m_dwOldProtect))
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualProtectEx);
		}
		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, pbBuffer, m_dwHookSize, NULL);//inlineHook

		WriteProcessMemory(hProcess, m_lpvAddress, abBufferCall, 0x9, NULL);//���ǵĹ��ܺ�������

		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress + 0x9), m_pOldBuffer, m_dwHookSize, NULL);//ԭ����ָ��
		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress + 0x9+ m_dwHookSize), abBufferCall+0x9, 0x5, NULL);//jmpָ��


		CloseHandle(hProcess);
		delete[]	pbBuffer;
	}
	void MyInlineHook32::m_ExitInlineHook()
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);

		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//��ԭInlineHook
		VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, m_dwOldProtect, NULL);
		VirtualFreeEx(hProcess, m_lpvAddress, 0, MEM_RELEASE);//�ͷ����ǵĴ���Ŀռ�

		delete[] m_pOldBuffer;
		//HOOK��ת�����ڴ�
		CloseHandle(hProcess);
	}

	void MyInlineHook32::m_WakeUpAllThread()
	{
		for (auto value : vechThreads)
		{
			ResumeThread(value);
			CloseHandle(value);
		}
		vechThreads.clear();
	}

	
}
