#include <REGX52.H>

void main(){

	while(1){
		if(P3_1 == 1)//独立按键按下时为0，没按下时为1
		{
			P2 = 0xFF; //1111 1111
		}
		else{
			P2 = 0xFE; //1111 1110
		}
	}
	
}