#include <REGX52.H>
#include "Buzzer.h"

/*
函数：Timer0Init
功能：定时器0初始化
输入：无
输出：无
*/
void Timer0Init(void)//1毫秒@11.0592MHz
{
	
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}


/*
//定时器中断函数
void Timer0_Routine() interrupt 1{
	static unsigned int T0Count;
	
	//重新赋初值保证计时为1us
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	
	T0Count++;
	if (T0Count >= 1000){
		T0Count = 0; //初始化计数
		//计数达到后的操作
		P2_0 = ~P2_0;
	}
	
}
*/

//定时器0中断函数（用于蜂鸣器方波生成）
void Timer0_Routine() interrupt 1
{
	if (Buzzer_Playing && Buzzer_Freq != 0)	//如果正在播放且频率有效
	{
		//加载当前频率对应的定时器初值
		TL0 = Buzzer_Freq % 256;		
		TH0 = Buzzer_Freq / 256;
		Buzzer = !Buzzer;	//翻转蜂鸣器引脚，产生方波
	}
	else	//停止播放时
	{
		Buzzer = 0;		//确保蜂鸣器关闭
	}
}