#include <REGX52.H>
#include "delay.h"

// 列选信号数组，依次选中第1-4列
static unsigned char code ColPins[] = {0x08, 0x04, 0x02, 0x01};
// 按键值映射表，对应行列交叉点的按键编号
static unsigned char code KeyMap[4][4] = {
    {1 ,  2,  3,  4},
    {5 ,  6,  7,  8},
    {9 , 10, 11, 12},
    {13, 14, 15, 16}
};

/**
 * @brief 读取矩阵键盘按键值
 * @return 按键值(1-16)，无按键按下时返回0
 */
unsigned char MatrixKey(void) {
    unsigned char col, row;
    unsigned char KeyNumber = 0;
    unsigned char rowVal;
    
    // 依次扫描每一列
    for(col = 0; col < 4; col++) {
        // 先将所有列置高，再将当前列置低
        P1 = 0xFF;
        P1 &= ~ColPins[col];
        
        // 读取行状态
        rowVal = (P1 >> 4) & 0x0F; // 取P1_7-P1_4的值
        
        // 检测每一行
        for(row = 0; row < 4; row++) {
            // 判断当前行是否有按键按下
            if(!(rowVal & (1 << (3 - row)))) {
                Delay1ms(20); // 消抖
                
                // 再次确认按键按下
                if(!(rowVal & (1 << (3 - row)))) {
                    // 等待按键释放
                    while(!(rowVal & (1 << (3 - row)))) {
                        rowVal = (P1 >> 4) & 0x0F;
                    }
                    Delay1ms(20); // 消抖
                    
                    // 获取对应的按键值
                    KeyNumber = KeyMap[row][col];
                    
                    // 退出所有循环，一次只处理一个按键
                    col = 4;
                    row = 4;
                }
            }
        }
    }
    
    return KeyNumber;
}
    