#pragma once

///////////////////////////////////////////////////////////////////////////////
#define M_MSGWINDOW TEXT("ZhiWuDaZhanJiangShiHelper")

///////////////////////////////////////////////////////////////////////////////
//自定义消息
#define UDM_AMOUNT_OF_SUNLIGHT	(WM_USER + 100) //自定义消息 修改阳光数量 （后跟数量）
#define UDM_INFINITE_SUNSHINE	(WM_USER + 101)	//无限阳光
#define UDM_NO_COOLING_TIME		(WM_USER + 110)	//无冷却时间

#define UDM_ZOMBIE_TAKEDOWN 	(WM_USER + 120)	//秒杀僵尸
#define UDM_BULLET_SPLIT		(WM_USER + 130)	//子弹分身
void m_FindMyWindow();