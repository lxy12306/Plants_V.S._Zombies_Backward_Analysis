#pragma once

/////////////////////////////////////////////////
/*阳光相关*/
//设置阳光数量
void m_FixTheSunLight(DWORD dwSunLightNumber);
//启动无限阳光
void m_InfiniteSunshine();
//结束无限阳光
void m_ExitInfiniteSunshine();
/////////////////////////////////////////////////
/*冷却时间相关*/
//启动无冷却时间
void m_NoCoolingTime();
//退出无冷却时间
void m_ExitNoCoolingTime();
/////////////////////////////////////////////////
/*僵尸相关相关*/
//启动秒杀僵尸
void m_KillZombiesWithOneHit();
//取消秒杀僵尸
void m_ExitKillZombiesWithOneHit();

void m_fJiangShiMiaoSha(void);//功能函数
/////////////////////////////////////////////////
/*子弹分身相关*/
void m_fZiDanFengSheng(void);//功能函数

//开启子弹分身
void m_ZiDanFengSheng();
//取消子弹分身
void m_ExitZiDanFengSheng();