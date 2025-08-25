#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

unsigned char KeyNum = 0;
unsigned char Count = 0;
unsigned int Password;

void main(){
	
	//LCD1602初始化
	LCD_Init();
	//提示输入密码
	LCD_ShowString(1,1,"Enter Password:");
	
	while(1){
		
		//将键值读取出
		KeyNum = MatrixKey();	
		
		if(KeyNum){
			//如果S1到S10按键按下，输入密码
			if(KeyNum <= 10){
				//记录输入次数，避免数据溢出
				if(Count < 4){
					//密码左移一位
					Password *= 10;
					//获取一位密码
					Password += KeyNum % 10;
					
					Count++;
				}
				
			}
			
			//如果S11按下，即确认密码
			if(KeyNum == 11){
				//设定正确密码
				if(Password == 1573){
					//表示密码正确
					LCD_ShowString(2,14,"OK ");
					//密码，计次重新初始化
					Password = 0;
					Count = 0;
				}else{
					//密码错误
					LCD_ShowString(2,14,"ERR");
					//密码，计次重新初始化
					Password = 0;
					Count = 0;
				}
				
			}
			
			//如果S12按下，则清零密码（取消键）
			if(KeyNum == 12){
				//密码，计次重新初始化
					Password = 0;
					Count = 0;
				//更新显示
				LCD_ShowNum(2,1,Password,4);
			}
			
			//更新显示
			LCD_ShowNum(2,1,Password,4);
			
		}
		
	}
	
}