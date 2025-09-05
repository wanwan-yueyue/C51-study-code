#include <REGX52.H>
#include "delay.h"

// 定义按键引脚
#define KEY1 P3_1
#define KEY2 P3_0
#define KEY3 P3_2
#define KEY4 P3_3

/**
 * 功能：扫描单个按键状态（通过函数参数区分不同按键）
 * 参数：
 *   keyIndex：按键编号（1-4，对应K1-K4）
 * 返回：按键按下时返回对应编号，否则返回0
 */
static unsigned char scanSingleKey(unsigned char keyIndex) {
    // 根据按键编号判断对应引脚状态
    bit pinState = 1;  // 默认为高电平（未按下）
    switch(keyIndex) {
        case 1: pinState = KEY1; break;  // 读取K1引脚
        case 2: pinState = KEY2; break;  // 读取K2引脚
        case 3: pinState = KEY3; break;  // 读取K3引脚
        case 4: pinState = KEY4; break;  // 读取K4引脚
    }

    // 按键按下检测（引脚为低电平）
    if (pinState == 0) {
        Delay1ms(20);  // 消抖
        // 再次检测确认按下
        switch(keyIndex) {
            case 1: pinState = KEY1; break;
            case 2: pinState = KEY2; break;
            case 3: pinState = KEY3; break;
            case 4: pinState = KEY4; break;
        }
        if (pinState == 0) {
            // 等待按键释放
            while(pinState == 0) {
                switch(keyIndex) {
                    case 1: pinState = KEY1; break;
                    case 2: pinState = KEY2; break;
                    case 3: pinState = KEY3; break;
                    case 4: pinState = KEY4; break;
                }
            }
            Delay1ms(20);  // 释放后消抖
            return keyIndex;  // 返回按键编号
        }
    }
    return 0;  // 未按下返回0
}

/**
 * 功能：获取独立按键键值
 * 返回：1-4（对应K1-K4），无按键按下返回0
 */
unsigned char Key() {
    unsigned char keyValue = 0;

    // 依次扫描4个按键（避免同时按下时冲突，优先识别先扫描的按键）
    if (keyValue == 0) {
        keyValue = scanSingleKey(1);  // 扫描K1
    }
    if (keyValue == 0) {
        keyValue = scanSingleKey(2);  // 扫描K2
    }
    if (keyValue == 0) {
        keyValue = scanSingleKey(3);  // 扫描K3
    }
    if (keyValue == 0) {
        keyValue = scanSingleKey(4);  // 扫描K4
    }

    return keyValue;
}