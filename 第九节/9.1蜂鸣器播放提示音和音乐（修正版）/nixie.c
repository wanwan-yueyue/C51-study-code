#include "nixie.h"
#include "delay.h"
#include <REGX52.H>

// 数码管段码表，存储在ROM中
const unsigned char NixieTable[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// 宏定义简化位操作
#define DIGIT_SEL_PORT P2
#define DIGIT_DATA_PORT P0

/**
 * @brief 设置数码管位置（内部函数）
 * @param location 位置参数(1-8)
 */
static void SetDigitPosition(unsigned char location)
{
    // 仅保留低3位，自动限制在0-7范围
    unsigned char pos_code = (location - 1) & 0x07;
    
    // 先关闭所有位选
    DIGIT_SEL_PORT &= ~(0x1C);  // 清除P2_4-P2_2位
    
    // 设置新的位选信号
    if(pos_code & 0x04) 
        DIGIT_SEL_PORT &= ~(1<<4); // 第2位
    else 
        DIGIT_SEL_PORT |= (1<<4);
    
    if(pos_code & 0x02) 
        DIGIT_SEL_PORT &= ~(1<<3); // 第1位
    else 
        DIGIT_SEL_PORT |= (1<<3);
    
    if(pos_code & 0x01) 
        DIGIT_SEL_PORT &= ~(1<<2); // 第0位
    else 
        DIGIT_SEL_PORT |= (1<<2);
}

/**
 * @brief 数码管显示函数
 * @param location 数码管位置(1-8)
 * @param number 要显示的数字(0-9)，超过9将显示横杠
 * @note 如果需要静态显示，可以关闭消隐处理
 */
void NixieDisplay(unsigned char location, unsigned char number)
{
	// 边界检查，非法位置不显示
	if(location < 1 || location > 8)
		return;
	
	// 数字越界处理，显示特殊符号(此处显示横杠)
	if(number > 9)
		number = 0x40; // 横杠的段码
	else
		number = NixieTable[number];
	
	// 关闭显示，防止位选切换时的串扰
	DIGIT_DATA_PORT = 0x00;
	
	// 设置位选
	SetDigitPosition(location);
	
	// 输出段码
	DIGIT_DATA_PORT = number;
	
	// 短暂延时，保证显示亮度
	Delay1ms(1);
	
	// 消影处理
	//DIGIT_DATA_PORT = 0x00;
}
    