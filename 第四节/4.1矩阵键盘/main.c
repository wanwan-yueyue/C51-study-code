#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

unsigned char KeyNum = 0;

void main(){
	
	//对LCD1602上电后进行初始化
	LCD_Init();
	//显示字符串，测试用
	LCD_ShowString(1,1,"Matrix Key:");
	while(1){
		
		KeyNum = MatrixKey();
		if(KeyNum){
			LCD_ShowNum(2,1,KeyNum,2);
		}
		
	}
	
}