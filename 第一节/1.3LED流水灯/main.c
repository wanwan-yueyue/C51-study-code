#include <REGX52.H>
#include <INTRINS.H>

void Delay500ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(){
	
	unsigned char led = 0x01; //1111 1110
	
	while(1){
		P2 = ~led; //取反， P2 = 0xFE
		
		//左移一位以实现流水灯效果
		led <<= 1;
		
		//当所有LED循环一遍后，重新从最低位开始
		if(led == 0x00){
			led = 0x01;
		}
		
		Delay500ms();
	}
}