#include <REGX52.H>


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
		// P2_0 = ~P2_0;
	}
}

*/