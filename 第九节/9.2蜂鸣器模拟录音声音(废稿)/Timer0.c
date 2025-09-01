#include <REGX52.H>
#include "Buzzer.h"


/*
函数：Timer0Init
功能：定时器0初始化
输入：无
输出：无
*/
void Timer0Init(void)	// 用于蜂鸣器方波生成（无需固定1ms，由频率动态调整）
{
    TMOD &= 0xF0;		// 清除定时器0配置
    TMOD |= 0x01;		// 16位定时模式
    TF0 = 0;			// 清除溢出标志
    ET0 = 1;			// 允许定时器0中断
    EA = 1;				// 允许总中断
    TR0 = 0;			// 初始不启动，由串口数据触发
}

// 定时器0中断：生成蜂鸣器所需方波
void Timer0_Routine() interrupt 1 {
    if(Buzzer_Playing && Buzzer_Freq != 0) {
        // 加载当前频率对应的初值（方波周期的1/2）
        TL0 = Buzzer_Freq % 256;		
        TH0 = Buzzer_Freq / 256;
        Buzzer = !Buzzer;  // 翻转引脚，产生方波
    } else {
        Buzzer = 0;  // 停止发声
    }
}