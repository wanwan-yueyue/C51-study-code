#include <REGX52.H>


/*
函数名：Uart_Init
功能：串口初始化
输入：无
输出：无
注意：4800bps@11.0592MHz
*/
void Uart_Init(void)	
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFA;			//设置定时初始值
	TH1 = 0xFA;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	//以下用以启动串口中断
	EA = 1;
	ES = 1;
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