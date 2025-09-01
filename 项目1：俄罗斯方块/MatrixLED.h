#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__

#include <REGX52.H>

// 引脚定义
#define MATRIX_LED_PORT    P0
sbit RCK = P3^5;  // RCLK
sbit SCK = P3^6;  // SRCLK
sbit SER = P3^4;  // SER

// 函数声明
void MatrixLED_Init(void);
void MatrixLED_WriteByte(unsigned char Byte);
void MatrixLED_ShowColumn(unsigned char Column, unsigned char Data);
void MatrixLED_ShowFrame(unsigned char *Frame);

#endif
