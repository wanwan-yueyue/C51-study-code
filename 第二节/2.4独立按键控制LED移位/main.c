#include <REGX52.H>
#include <INTRINS.H>

unsigned char LEDNum = 0;

void Delay(unsigned int xms)	//@11.0592MHz
{
	unsigned char data i, j;

	while(xms--){
		
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
		
	}
	
}

void main(){
	//初始化
	P2 =~0x01;
	
	while(1){
			
		//按下独立按键K1，LED灯向高位移动
		if(P3_1 == 0){
			//消抖
			Delay(20);
			while(P3_1 == 0);
			Delay(20);
			
			LEDNum++;
			//当移位值超过8时，将移位值重新赋值为0
			if(LEDNum >= 8){
				LEDNum = 0;
			}
			
			//对P2值进行移位操作,注意取反操作
			P2 =~ (0x01 << LEDNum);
		}
		
		//按下独立按键K2，LED灯向低位移动
		if(P3_0 == 0){
			//消抖
			Delay(20);
			while(P3_0 == 0);
			Delay(20);
			
			//先判断，避免越界
			if(LEDNum == 0){
				LEDNum = 7;
			}
			else{
				LEDNum--;
			}
			
			//对P2值进行移位操作
			P2 =~ (0x01 << LEDNum);
		}
		
	}
	
}