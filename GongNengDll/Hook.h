#pragma once
#include <Windows.h>
#include <vector>
namespace LXY
{
	class MyInlineHook32
	{
		typedef void (*m_func)(void);
		/*友元声明以及using定义*/
	private:
		/*私有数据成员*/
		std::vector<HANDLE> vechThreads;
		DWORD dwPid ;	//要HOOK的进程的id
		DWORD dwMyThreadId;
		BYTE abBuffer[0x5] = {0xE9,0x00,0x00,0x00,0x00};
		BYTE abBufferCall[0x100] = { 0x60/*pushad*/,0x9c/*pushfd*/,
			0xE8,0x00,0x00,0x00,0x00,/*call ???*/
			0x9d/*popfd*/,
			0x61/*popad*/,
			0xE9,0x00,0x00,0x00,0x00/*jmp???*/ /*9*/};
		//BYTE abBufferCall[0x100] = { 0x60/*pushad*/,0x9c/*pushfd*/,
		//0xBB,0x00,0x00,0x00,0x00/*mov ebx???*/,
		//0xFF,0xD3,/*call ebx*/
		//0x9d/*popfd*/,
		//0x61/*popad*/,
		//0xE9,0x00,0x00,0x00,0x00/*jmp???*/ /*11*/ };

		DWORD m_dwHookBeGin;	//Hook的地址
		DWORD m_dwHookSize;		//Hook覆盖的长度
		m_func m_HookFunc;		//Hook后调用的函数

		LPVOID m_lpvAddress;	//申请的InlineHook的空间
		BYTE* m_pOldBuffer;		//保存原来的地址
		DWORD m_dwOldProtect;	//保存原来的地址的保护属性
	public:
		/*公有数据成员*/

	public:
		/*构造函数与析构函数*/
		MyInlineHook32(DWORD dwHookBeGin,DWORD dwHookSize, m_func f):m_dwHookBeGin(dwHookBeGin), m_dwHookSize(dwHookSize), m_HookFunc(f),
			dwMyThreadId(GetCurrentThreadId()){
			dwPid = GetCurrentProcessId();
		}
		MyInlineHook32(DWORD dwProcessId, DWORD dwHookBeGin, DWORD dwHookSize,m_func f):m_dwHookBeGin(dwHookBeGin), m_dwHookSize(dwHookSize),m_HookFunc(f),
			dwPid(dwProcessId),dwMyThreadId(GetCurrentThreadId()) {
		}
		~MyInlineHook32() {
			if(!vechThreads.empty())
				m_WakeUpAllThread();
		}
	public:
		/*导出函数*/
		void e_InlineHook1()//先执行覆盖的指令
		{
			m_StopAllThreadExpect(dwMyThreadId);
			m_InlineHook1();
			m_WakeUpAllThread();
		}
		void e_InlineHook2()//后执行覆盖的指令
		{
			m_StopAllThreadExpect(dwMyThreadId);
			m_InlineHook2();
			m_WakeUpAllThread();
		}
		void e_ExitInlineHook()
		{
			m_StopAllThreadExpect(dwMyThreadId);
			m_ExitInlineHook();
			m_WakeUpAllThread();
		}
	private:
		/*其他非导出功能函数*/
		void m_StopAllThreadExpect(unsigned dwThreadId = 0);
		void m_InlineHook1();//占用的地方被先调用
		void m_InlineHook2();//占用的地方被后调用
		void m_ExitInlineHook();
		void m_WakeUpAllThread();
		void m_FixHook1()
		{

			DWORD dwJmpOffset = (DWORD)m_lpvAddress - (m_dwHookBeGin + 5);
			memcpy_s(abBuffer + 1, 4, &dwJmpOffset, 4);//修正跳转地址

			dwJmpOffset = (DWORD)m_HookFunc - ((DWORD)m_lpvAddress + 5 + 2+m_dwHookSize);
			memcpy_s(abBufferCall+3, 4, &dwJmpOffset, 4);//修正调用函数

			dwJmpOffset = (m_dwHookBeGin)- ((DWORD)m_lpvAddress + 11 + 5);
			memcpy_s(abBufferCall + 10, 4, &dwJmpOffset, 4);//修正跳回地址
		}
		void m_FixHook2()
		{

			DWORD dwJmpOffset = (DWORD)m_lpvAddress - (m_dwHookBeGin + 5);
			memcpy_s(abBuffer + 1, 4, &dwJmpOffset, 4);//修正跳转地址

			dwJmpOffset = (DWORD)m_HookFunc - ((DWORD)m_lpvAddress + 5 + 2);
			memcpy_s(abBufferCall + 3, 4, &dwJmpOffset, 4);//修正调用函数

			dwJmpOffset = (m_dwHookBeGin)-((DWORD)m_lpvAddress + 11 + 5);
			memcpy_s(abBufferCall + 10, 4, &dwJmpOffset, 4);//修正跳回地址
		}
	};

}
