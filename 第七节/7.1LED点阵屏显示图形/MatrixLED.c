#include "MatrixLED.h"
#include "delay.h"

/**
 * @brief 向74HC595写入一个字节
 * @param Byte 要写入的字节
 */
void _74HC595_WriteByte(unsigned char Byte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        SER = Byte & (0x80 >> i);
        SCK = 1;
        SCK = 0;
    }
    RCK = 1;
    RCK = 0;
}

/**
 * @brief 初始化点阵屏
 */
void MatrixLED_Init(void)
{
    SCK = 0;
    RCK = 0;
}

/**
 * @brief 显示一列数据
 * @param Column 列号(0-7)
 * @param Data 列数据(1为亮，0为灭)
 */
void MatrixLED_ShowColumn(unsigned char Column, unsigned char Data)
{
    _74HC595_WriteByte(Data);
    MATRIX_LED_PORT = ~(0x80 >> Column);
    Delay1ms(1);
    MATRIX_LED_PORT = 0xFF;
}

/**
 * @brief 显示一帧完整图像
 * @param Frame 指向帧数据的指针，需包含8个字节
 */
void MatrixLED_ShowFrame(const unsigned char *Frame)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        MatrixLED_ShowColumn(i, Frame[i]);
    }
}

/**
 * @brief 播放动画
 * @param Animation 指向动画数据的指针
 * @param FrameCount 动画帧数
 * @param Speed 动画速度(数值越大越慢)
 */
void MatrixLED_PlayAnimation(const unsigned char *Animation, unsigned char FrameCount, unsigned int Speed)
{
    static unsigned char Offset = 0;
    static unsigned int Count = 0;
    unsigned char i;
    
    // 显示当前帧的8列数据
    for(i = 0; i < 8; i++)
    {
        MatrixLED_ShowColumn(i, Animation[i + Offset]);
    }
    
    // 速度控制
    Count++;
    if(Count > Speed)
    {
        Count = 0;
        Offset += 8;  // 偏移8位，切换到下一帧
        
        // 动画播放完毕，回到第一帧
        if(Offset >= FrameCount * 8)
        {
            Offset = 0;
        }
    }
}
