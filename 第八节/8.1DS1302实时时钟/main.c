#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"


void main(){
	
	//LCD1602初始化
	LCD_Init();
	//DS1302初始化
	DS1302_Init();
	//测试LCD1602
	//LCD_ShowString(1,1,"RTC");
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  :  :  ");

	//设置时间
	DS1302_SetTime();
	
	while(1){
		
		//读取时间
		DS1302_ReadTime();
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
		LCD_ShowNum(1, 4, DS1302_Time[1], 2);
		LCD_ShowNum(1, 7, DS1302_Time[2], 2);
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
		LCD_ShowNum(2, 4, DS1302_Time[4], 2);
		LCD_ShowNum(2, 7, DS1302_Time[5], 2);
		
		
	}
	
}