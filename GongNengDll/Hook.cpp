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
		mti.e_QueryThreadInfo_By_Pid(dwPid, vlst); //列举出线程消息

		vechThreads.clear();
		for (auto value : vlst)
		{
			if (value.dwTid == dwThreadId)
				continue; //自己这个线程不暂停
			auto hThread = ::OpenThread(THREAD_ALL_ACCESS,false,value.dwTid);
			if (hThread == NULL) //已经能列举线程了 所以打开线程句柄是由权限的 所以出错的化可能线程消失了
				continue;
			vechThreads.emplace_back(hThread);
		}
		for (auto value : vechThreads)
		{
			SuspendThread(value);
		}
	}

	void MyInlineHook32::m_InlineHook1() //占用的地方被先调用
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);
		m_lpvAddress = VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_lpvAddress == NULL)
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualAllocEx);
		}
		//HOOK跳转到的内存

		m_FixHook1();
		BYTE* pbBuffer = new BYTE[m_dwHookSize];
		memset(pbBuffer, 0x90, m_dwHookSize);
		memcpy_s(pbBuffer, m_dwHookSize, abBuffer, 5); //HOOK处多余字节使用nop填充

		m_pOldBuffer = new BYTE[m_dwHookSize];
		ReadProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//保存原来的HOOK的东西
		if (!VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, PAGE_EXECUTE_READWRITE, &m_dwOldProtect))
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualProtectEx);
		}
		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, pbBuffer, m_dwHookSize, NULL);//inlineHook

		WriteProcessMemory(hProcess, m_lpvAddress, m_pOldBuffer, m_dwHookSize, NULL);//原来的函数
		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress+ m_dwHookSize), abBufferCall, 0x100, NULL);

		CloseHandle(hProcess);
		delete[]	pbBuffer;
	}
	void MyInlineHook32::m_InlineHook2() //占用的地方被先调用
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);
		m_lpvAddress = VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_lpvAddress == NULL)
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualAllocEx);
		}
		//HOOK跳转到的内存

		m_FixHook2();
		BYTE* pbBuffer = new BYTE[m_dwHookSize];
		memset(pbBuffer, 0x90, m_dwHookSize);
		memcpy_s(pbBuffer, m_dwHookSize, abBuffer, 5); //HOOK处多余字节使用nop填充

		m_pOldBuffer = new BYTE[m_dwHookSize];
		ReadProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//保存原来的HOOK的东西
		if (!VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, PAGE_EXECUTE_READWRITE, &m_dwOldProtect))
		{
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::VirtualProtectEx);
		}
		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, pbBuffer, m_dwHookSize, NULL);//inlineHook

		WriteProcessMemory(hProcess, m_lpvAddress, abBufferCall, 0x9, NULL);//我们的功能函数部分

		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress + 0x9), m_pOldBuffer, m_dwHookSize, NULL);//原来的指令
		WriteProcessMemory(hProcess, (LPVOID)((DWORD)m_lpvAddress + 0x9+ m_dwHookSize), abBufferCall+0x9, 0x5, NULL);//jmp指令


		CloseHandle(hProcess);
		delete[]	pbBuffer;
	}
	void MyInlineHook32::m_ExitInlineHook()
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
		if (hProcess == NULL)
			MD_Throw_Windows(MyInlineHook32::m_InlineHook::OpenProcess);

		WriteProcessMemory(hProcess, (LPVOID)m_dwHookBeGin, m_pOldBuffer, m_dwHookSize, NULL);//还原InlineHook
		VirtualProtectEx(hProcess, (LPVOID)m_dwHookBeGin, m_dwHookSize, m_dwOldProtect, NULL);
		VirtualFreeEx(hProcess, m_lpvAddress, 0, MEM_RELEASE);//释放我们的代码的空间

		delete[] m_pOldBuffer;
		//HOOK跳转到的内存
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
