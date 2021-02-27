#pragma once
#include <Windows.h>
#include <vector>
namespace LXY
{
	class MyInlineHook32
	{
		typedef void (*m_func)(void);
		/*��Ԫ�����Լ�using����*/
	private:
		/*˽�����ݳ�Ա*/
		std::vector<HANDLE> vechThreads;
		DWORD dwPid ;	//ҪHOOK�Ľ��̵�id
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

		DWORD m_dwHookBeGin;	//Hook�ĵ�ַ
		DWORD m_dwHookSize;		//Hook���ǵĳ���
		m_func m_HookFunc;		//Hook����õĺ���

		LPVOID m_lpvAddress;	//�����InlineHook�Ŀռ�
		BYTE* m_pOldBuffer;		//����ԭ���ĵ�ַ
		DWORD m_dwOldProtect;	//����ԭ���ĵ�ַ�ı�������
	public:
		/*�������ݳ�Ա*/

	public:
		/*���캯������������*/
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
		/*��������*/
		void e_InlineHook1()//��ִ�и��ǵ�ָ��
		{
			m_StopAllThreadExpect(dwMyThreadId);
			m_InlineHook1();
			m_WakeUpAllThread();
		}
		void e_InlineHook2()//��ִ�и��ǵ�ָ��
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
		/*�����ǵ������ܺ���*/
		void m_StopAllThreadExpect(unsigned dwThreadId = 0);
		void m_InlineHook1();//ռ�õĵط����ȵ���
		void m_InlineHook2();//ռ�õĵط��������
		void m_ExitInlineHook();
		void m_WakeUpAllThread();
		void m_FixHook1()
		{

			DWORD dwJmpOffset = (DWORD)m_lpvAddress - (m_dwHookBeGin + 5);
			memcpy_s(abBuffer + 1, 4, &dwJmpOffset, 4);//������ת��ַ

			dwJmpOffset = (DWORD)m_HookFunc - ((DWORD)m_lpvAddress + 5 + 2+m_dwHookSize);
			memcpy_s(abBufferCall+3, 4, &dwJmpOffset, 4);//�������ú���

			dwJmpOffset = (m_dwHookBeGin)- ((DWORD)m_lpvAddress + 11 + 5);
			memcpy_s(abBufferCall + 10, 4, &dwJmpOffset, 4);//�������ص�ַ
		}
		void m_FixHook2()
		{

			DWORD dwJmpOffset = (DWORD)m_lpvAddress - (m_dwHookBeGin + 5);
			memcpy_s(abBuffer + 1, 4, &dwJmpOffset, 4);//������ת��ַ

			dwJmpOffset = (DWORD)m_HookFunc - ((DWORD)m_lpvAddress + 5 + 2);
			memcpy_s(abBufferCall + 3, 4, &dwJmpOffset, 4);//�������ú���

			dwJmpOffset = (m_dwHookBeGin)-((DWORD)m_lpvAddress + 11 + 5);
			memcpy_s(abBufferCall + 10, 4, &dwJmpOffset, 4);//�������ص�ַ
		}
	};

}
