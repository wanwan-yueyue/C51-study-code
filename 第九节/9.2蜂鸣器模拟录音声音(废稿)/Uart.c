#include <REGX52.H>
#include "Buzzer.h"
#include "AudioProcess.h"

// 全局变量：存储当前音频数据
unsigned char g_pcm_data = 0;

/*
函数名：Uart_Init
功能：串口初始化
输入：无
输出：无
注意：9600bps@11.0592MHz
*/
void Uart_Init(void)	// 9600bps@11.0592MHz（修改波特率以匹配数据传输）
{
    PCON &= 0x7F;		// 波特率不倍速
    SCON = 0x50;		// 8位数据，允许接收
    TMOD &= 0x0F;		// 清除定时器1配置
    TMOD |= 0x20;		// 定时器1工作在方式2（自动重装载）
    TL1 = 0xFD;			// 9600bps初值（11.0592MHz）
    TH1 = 0xFD;			
    ET1 = 0;			// 禁止定时器1中断
    TR1 = 1;			// 启动定时器1
    EA = 1;				// 允许总中断
    ES = 1;				// 允许串口中断
}


/*
函数名：Uart_SendByte
功能：向串口数据缓存寄存器写入一个字节的数据
输入：Byte 要输入的那一个字节的数据
输出：无
*/
void Uart_SendByte(unsigned char Byte){
	
	//将字节写入串口数据缓存寄存器
	SBUF = Byte;
	//检查是否写入,发送请求中断标志位置1时继续下一步
	while(TI == 0);
	//复位TI
	TI = 0;
	
}


// 串口接收中断：实时处理PCM数据
void Uart_Routine() interrupt 4 {
		unsigned int freq;
    if(RI) {  // 接收完成
				P2 = ~P2;  //LED调试
        RI = 0;  // 清除标志位
        g_pcm_data = SBUF;  // 读取PCM数据（0~255）
        
        // 转换为蜂鸣器频率并更新定时器
        freq = PCM_To_Freq(g_pcm_data);
        Buzzer_Freq = CalculateTimerReload(freq);  // 复用蜂鸣器模块的初值计算函数
        Buzzer_Playing = 1;  // 启动蜂鸣器
        TR0 = 1;  // 确保定时器0启动
    }
}


/*

//串口中断函数
void Uart_Routine() interrupt 4
{
	//判断是不是接收中断
	if(RI == 1){
		
		//P2 = ~SBUF;
		
		//接收中断请求标志位RI置零复位
		RI = 0;
		
	}
	
}
*/
