#ifndef __DS1302_H__
#define __DS1302_H__

#include <REGX52.H>

// 外部可见的时间数组
extern unsigned char DS1302_Time[];

// 引脚定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

// 原有函数声明
void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command, unsigned char Data);
unsigned char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime(void);
void DS1302_ReadTime(void);

#endif