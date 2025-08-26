#include <REGX52.H>
#include "delay.h"
#include "Uart.h"


unsigned char SecNum = 0;



void main(){
	
	//串口初始化
	Uart_Init();
	
	while(1){
		
		Uart_SendByte(SecNum);
		SecNum++;
		Delay1ms(1000);
		
	}
	
}