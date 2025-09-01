#include <INTRINS.H>
#include "delay.h"

/**
 * @brief 延时函数（约1ms）
 * @param xms 延时时长，单位为毫秒
 * @note 基于11.0592MHz晶振
 */
void Delay1ms(unsigned int xms)	//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
