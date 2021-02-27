#include "pch.h"
//一只普通僵尸的血量是270

#define JS_XUELIANG_OFFSET 0xC8 //血量偏移
#define JS_HUJIA1_OFFSET 0xD0   //护甲1偏移
#define JS_HUJIA2_OFFSET 0xD8   //护甲2偏移
#define JS_ShiFou_Bei_ShuaXing 0xEC //是否刷新标志位偏移
//c8偏移为血量 cc偏移为最高血量 血量降为50则死亡
//d0偏移为护甲 dd偏移为最大护甲
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
    if(*PDWORD((DWORD)pdwJiangShi + JS_XUELIANG_OFFSET) >= 50) //还没有被削弱的
        *PDWORD((DWORD)pdwJiangShi + JS_XUELIANG_OFFSET) = 30;
    //不同僵尸 有不同的的护甲  需要区别处理
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
