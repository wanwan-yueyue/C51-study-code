#include <REGX52.H>
#include "Timer0.h"
#include "delay.h"
#include "LCD1602.h"

//定义变量“秒”
unsigned char Sec = 0, Min = 0, Hour = 0;

void main(){
	
	//LCD1602屏幕初始化
	LCD_Init();
	//定时器初始化
	Timer0Init();
	
	LCD_ShowString(1,1,"Clock:");
	LCD_ShowString(2,1,"  :  :");

	while(1){
		
		//显示时
		LCD_ShowNum(2,1,Hour,2);
		//显示分
		LCD_ShowNum(2,4,Min,2);
		//显示秒
		LCD_ShowNum(2,7,Sec,2);
		
	}
	
}

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
		Sec++;
		
		//时间进位
		if(Sec >= 60){
			Sec = 0;
			Min++;
		}
		if(Min >= 60){
			Min = 0;
			Hour++;
		}
		if(Hour >= 24)
			Hour = 0;
	}
}