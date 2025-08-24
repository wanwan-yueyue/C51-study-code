#include <REGX52.H>
#include <INTRINS.H>

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
	
	unsigned char LEDNum = 0; //0000 0000
	
	while(1){
		
		if(P3_1 == 0){
			//按键消抖
			Delay(20);
			while(P3_1 == 0);
			Delay(20);
			
			/*
			P2++; //P2初始值是 1111 1111
			P2 =~ P2;
			*/
			
			LEDNum++; //0000 0001
			P2 =~ LEDNum; //1111 1110
			
			/*
			或者使用
			P2--;
			这样P2从 1111 1111 会变为 1111 1110，然后再减少为1111 1101
			*/
		}
		
	}
	
}