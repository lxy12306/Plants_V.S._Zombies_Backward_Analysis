#include "pch.h"
//һֻ��ͨ��ʬ��Ѫ����270

#define JS_XUELIANG_OFFSET 0xC8 //Ѫ��ƫ��
#define JS_HUJIA1_OFFSET 0xD0   //����1ƫ��
#define JS_HUJIA2_OFFSET 0xD8   //����2ƫ��
#define JS_ShiFou_Bei_ShuaXing 0xEC //�Ƿ�ˢ�±�־λƫ��
//c8ƫ��ΪѪ�� ccƫ��Ϊ���Ѫ�� Ѫ����Ϊ50������
//d0ƫ��Ϊ���� ddƫ��Ϊ��󻤼�
static bool g_bIsMiaosha = false;

void __declspec(naked) m_fJiangShiMiaoSha(void) 
{
    __asm
    {
        push ebp;
        mov ebp, esp;
        sub esp, 0x10;
    }
    PDWORD pdwJiangShi;
    _asm
    {
        mov pdwJiangShi, eax
    }

    __asm
    {
        mov bl,g_bIsMiaosha
        test bl,bl
        jne Func1
        leave;
        ret;
    }

Func1:
    if(*PDWORD((DWORD)pdwJiangShi + JS_XUELIANG_OFFSET) >= 50) //��û�б�������
        *PDWORD((DWORD)pdwJiangShi + JS_XUELIANG_OFFSET) = 30;
    //��ͬ��ʬ �в�ͬ�ĵĻ���  ��Ҫ������
    *PDWORD((DWORD)pdwJiangShi + JS_HUJIA1_OFFSET) = 0;
    *PDWORD((DWORD)pdwJiangShi + JS_HUJIA2_OFFSET) = 0;
    __asm
    {
        leave;
        ret;
    }
}


void m_KillZombiesWithOneHit()
{
    g_bIsMiaosha = true;
}

void m_ExitKillZombiesWithOneHit()
{
    g_bIsMiaosha = false;
}
