#ifndef __DS1302__H_
#define __DS1302__H_


extern unsigned char DS1302_Time[];


//DS1302初始化
void DS1302_Init(void);
//DS1302写入字节
void DS1302_WriteByte(unsigned char Command,Data);
//DS1302读取字节
unsigned char DS1302_ReadByte(unsigned char Command);
//设置时间
void DS1302_SetTime(void);
//读取时间
void DS1302_ReadTime(void);

#endif