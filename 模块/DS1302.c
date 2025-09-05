#include "DS1302.h"

// 寄存器地址（内部常量，不暴露）
#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR			0x84
#define DS1302_DATE			0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY			0x8A
#define DS1302_YEAR			0x8C
#define DS1302_WP			0x8E

// 时间数组
unsigned char DS1302_Time[] = {25, 8, 2, 00, 00, 00, 6};  // 年/月/日/时/分/秒/星期

// 内部辅助函数（静态，仅本文件可见）
static unsigned char dec_to_bcd(unsigned char dec);  // 十进制转BCD
static unsigned char bcd_to_dec(unsigned char bcd);  // BCD转十进制


/**
 * 初始化DS1302
 */
void DS1302_Init(void){
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}


/**
 * 写入字节到DS1302
 * @param Command：寄存器地址
 * @param Data：要写入的数据
 */
void DS1302_WriteByte(unsigned char Command, unsigned char Data){
	unsigned char i;
	DS1302_CE = 1;  // 使能芯片
	
	// 写入地址（低位在前）
	for(i = 0; i < 8; i++){
		DS1302_IO = (Command >> i) & 0x01;  // 取第i位
		DS1302_SCLK = 1;  // 时钟上升沿写入
		DS1302_SCLK = 0;
	}
	
	// 写入数据（低位在前）
	for(i = 0; i < 8; i++){
		DS1302_IO = (Data >> i) & 0x01;  // 取第i位
		DS1302_SCLK = 1;  // 时钟上升沿写入
		DS1302_SCLK = 0;
	}
	
	DS1302_CE = 0;  // 禁用芯片
}


/**
 * 从DS1302读取字节
 * @param Command：寄存器地址
 * @return 读取到的数据
 */
unsigned char DS1302_ReadByte(unsigned char Command){
	unsigned char i, Data = 0;
	Command |= 0x01;  // 读操作需置位最低位
	DS1302_CE = 1;    // 使能芯片
	
	// 写入地址（低位在前）
	for(i = 0; i < 8; i++){
		DS1302_IO = (Command >> i) & 0x01;  // 取第i位
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;  // 时钟下降沿准备读取
	}
	
	// 读取数据（低位在前）
	for(i = 0; i < 8; i++){
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;  // 时钟下降沿读取
		if(DS1302_IO) Data |= (1 << i);  // 保存第i位
	}
	
	DS1302_CE = 0;    // 禁用芯片
	DS1302_IO = 0;    // 复位IO口
	return Data;
}


/**
 * 设置时间
 * 功能：将DS1302_Time数组的时间写入芯片
 */
void DS1302_SetTime(void){
	// 关闭写保护
	DS1302_WriteByte(DS1302_WP, 0x00);
	
	// 十进制转BCD后写入
	DS1302_WriteByte(DS1302_YEAR, dec_to_bcd(DS1302_Time[0]));
	DS1302_WriteByte(DS1302_MONTH, dec_to_bcd(DS1302_Time[1]));
	DS1302_WriteByte(DS1302_DATE, dec_to_bcd(DS1302_Time[2]));
	DS1302_WriteByte(DS1302_HOUR, dec_to_bcd(DS1302_Time[3]));
	DS1302_WriteByte(DS1302_MINUTE, dec_to_bcd(DS1302_Time[4]));
	DS1302_WriteByte(DS1302_SECOND, dec_to_bcd(DS1302_Time[5]));
	DS1302_WriteByte(DS1302_DAY, dec_to_bcd(DS1302_Time[6]));
	
	// 打开写保护
	DS1302_WriteByte(DS1302_WP, 0x80);
}


/**
 * 读取时间
 * 功能：从芯片读取时间到DS1302_Time数组
 */
void DS1302_ReadTime(void){
	unsigned char temp;
	
	// 读取BCD码并转为十进制
	temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = bcd_to_dec(temp);
	
	temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = bcd_to_dec(temp);
}


/**
 * 内部辅助函数：十进制转BCD码（如25 → 0x25）
 */
static unsigned char dec_to_bcd(unsigned char dec){
	return (dec / 10) << 4 | (dec % 10);
}


/**
 * 内部辅助函数：BCD码转十进制（如0x25 → 25）
 */
static unsigned char bcd_to_dec(unsigned char bcd){
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}