#include <REGX52.H>
#include <INTRINS.H>
#include "Timer0.h"
#include "Key.h"

unsigned char KeyNum = 0;
unsigned char LEDMode = 0;

void main(){
	P2 = 0xFE;
	Timer0Init(); //定时器初始化
	
	while(1){
		
		//获取按键键码
		KeyNum = Key();
		
		if(KeyNum){
			//键码值为1时（K1按下）
			if(KeyNum == 1){
				LEDMode++;
				if(LEDMode >= 2) LEDMode = 0;
			}
			
		}
		
	}
	
}

void Timer0_Routine() interrupt 1{
	static unsigned int T0Count;
	
	//重新赋初值保证计时为1us
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	
	T0Count++;
	if (T0Count >= 500){
		T0Count = 0; //初始化计数
		
		//模式0,左移
		if(LEDMode == 0){
			P2 = _crol_(P2,1);
		}
		//模式1,右移
		if(LEDMode == 1){
			P2 = _cror_(P2,1);
		}
		
	}
	
}