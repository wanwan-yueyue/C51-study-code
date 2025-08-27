#include <REGX52.H>
#include "MatrixLED.h"
#include "delay.h"

// 定义显示图案（笑脸）
unsigned char code MatrixLED_Frame[] = {
    0x3C,  // 列0数据
    0x42,  // 列1数据
    0xA9,  // 列2数据
    0x85,  // 列3数据
    0x85,  // 列4数据
    0xA9,  // 列5数据
    0x42,  // 列6数据
    0x3C   // 列7数据
};

void main(void)
{
    MatrixLED_Init();  // 初始化点阵屏
    
    while(1)
    {
        // 显示一帧图像
        MatrixLED_ShowFrame(MatrixLED_Frame);
    }
}
