#include "MatrixLED.h"
#include "delay.h"

/**
 * @brief  初始化点阵屏
 * @param  无
 * @retval 无
 */
void MatrixLED_Init(void)
{
    SCK = 0;
    RCK = 0;
}

/**
 * @brief  向74HC595写入一个字节
 * @param  Byte 要写入的字节
 * @retval 无
 */
void MatrixLED_WriteByte(unsigned char Byte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        SER = Byte & (0x80 >> i);  // 取出当前位数据
        SCK = 1;                   // 移位时钟线拉高，数据移入
        SCK = 0;                   // 移位时钟线拉低，准备下一次移位
    }
    RCK = 1;                       // 存储时钟线拉高，数据输出
    RCK = 0;                       // 存储时钟线拉低，准备下一次输出
}

/**
 * @brief  显示点阵屏的一列数据
 * @param  Column 列号，范围0-7，0为最左边
 * @param  Data 列数据，高位在上，低位在下，1为亮，0为灭
 * @retval 无
 */
void MatrixLED_ShowColumn(unsigned char Column, unsigned char Data)
{
    MatrixLED_WriteByte(Data);               // 写入列数据
    MATRIX_LED_PORT = ~(0x80 >> Column);     // 选择列（低电平有效）
    Delay1ms(1);                             // 延时1ms，保证亮度
    MATRIX_LED_PORT = 0xFF;                  // 关闭所有列，防止串扰
}

/**
 * @brief  显示一帧完整的点阵图像
 * @param  Frame 指向图像数据的指针，包含8个字节，每个字节代表一列
 * @retval 无
 */
void MatrixLED_ShowFrame(unsigned char *Frame)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        MatrixLED_ShowColumn(i, Frame[i]);
    }
}
