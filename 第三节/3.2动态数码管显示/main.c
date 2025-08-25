#include <REGX52.H>
#include <INTRINS.H>
#include "delay.h"
#include "nixie.h"

void main(void)
{
	while(1)
	{
		// 循环刷新显示，避免亮度不足
		NixieDisplay(1, 1);
		NixieDisplay(2, 2);
		NixieDisplay(3, 3);
	}
}
