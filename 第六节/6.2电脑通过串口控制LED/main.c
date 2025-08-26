#include <REGX52.H>
#include "delay.h"
#include "Uart.h"


unsigned char SecNum = 0;



void main(){
	
	//串口初始化
	Uart_Init();
	
	while(1){
		
		
		
	}
	
}

void Uart_Routine() interrupt 4
{
	//判断是不是接收中断
	if(RI == 1){
		
		P2 = ~SBUF;
		Uart_SendByte(SBUF);
		//接收中断请求标志位RI置零复位
		RI = 0;
		
	}
	
}

//注意，一个函数不能同时出现在主函数和中断函数中 