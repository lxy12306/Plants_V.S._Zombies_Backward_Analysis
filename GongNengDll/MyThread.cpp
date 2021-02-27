#include "pch.h"
#include "F:\Code\object\ProcessAndHwndAndThread\MyThread.h"
#include "F:\Code\object\Message\MyPrint.h"
#include <Tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")
namespace LXY
{
	BOOL MyThreadInfo::e_QueryThreadInfo_By_Pid(__in DWORD dwPid, __out std::vector<LXY_PROCESS_THREADINFO>& vlst)
	{
		ZwQueryInformationThread pZwQueryInformationThread = NULL;
		auto fnSetThreadInfo_By_Tid = [&pZwQueryInformationThread](__in DWORD dwTid, __out LXY_PROCESS_THREADINFO& ThreadInfo)
		{
			THREAD_BASIC_INFORMATION    tbi = { 0 };
			PVOID                       pvStartAddr = NULL;
			LONG                        lnStatus = NULL;
			HANDLE                      hThread = NULL;
			HANDLE						hProcess = NULL;

			hThread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
			if (hThread == NULL)
				return FALSE;

			lnStatus = pZwQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &pvStartAddr, sizeof(pvStartAddr), NULL);
			if (lnStatus < 0)
			{
				CloseHandle(hThread);
				return FALSE;
			}

			ZeroMemory(&ThreadInfo, sizeof(ThreadInfo));
			ThreadInfo.dwTid = dwTid;
			ThreadInfo.pvStartAddr = pvStartAddr;

			lnStatus = pZwQueryInformationThread(hThread, ThreadBasicInformation, &tbi, sizeof(tbi), NULL);
			if (lnStatus < 0)
			{
				CloseHandle(hThread);
				return FALSE;
			};

			hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)tbi.ClientId.UniqueProcess);
			if (hProcess == NULL)
			{
				CloseHandle(hThread);
				return FALSE;
			};

			GetMappedFileName(hProcess, pvStartAddr, ThreadInfo.wszModuleName, MAX_PATH);

			CloseHandle(hProcess);
			CloseHandle(hThread);
			return TRUE;
		};

		try
		{
			HMODULE hmNtDLL = ::GetModuleHandleW(L"ntdll.dll");
			if (hmNtDLL == NULL)
				return FALSE;

			pZwQueryInformationThread = (ZwQueryInformationThread)::GetProcAddress(hmNtDLL, "ZwQueryInformationThread");
			if (pZwQueryInformationThread == NULL)
				return FALSE;

			static LXY_PROCESS_THREADINFO ThreadInfo;
			HANDLE hSnapshot = NULL;
			THREADENTRY32 te = { 0 };
			te.dwSize = sizeof(te);

			hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			if (Thread32First(hSnapshot, &te))
			{
				do
				{
					if (te.th32OwnerProcessID == dwPid && fnSetThreadInfo_By_Tid(te.th32ThreadID, ThreadInfo))
						vlst.push_back(ThreadInfo);
				} while (Thread32Next(hSnapshot, &te));
			};
			CloseHandle(hSnapshot);
			return vlst.size() != NULL ? TRUE : FALSE;
		}
		catch (...)
		{
			LXY::m_PrintDebug("MyThreadInfo::e_QueryThreadInfo_By_Pid Error");
		}
		return FALSE;
	}
}