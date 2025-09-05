#ifndef __PASSWORD_INPUT_H__
#define __PASSWORD_INPUT_H__

#include <REGX52.H>

// 按键功能宏定义（与硬件对应）
#define KEY_1        1   // 数字1
#define KEY_2        2   // 数字2
#define KEY_3        3   // 数字3
#define KEY_BACKSPACE 4  // 退格键
#define KEY_4        5   // 数字4
#define KEY_5        6   // 数字5
#define KEY_6        7   // 数字6
#define KEY_CLEAR    8   // 清除键
#define KEY_7        9   // 数字7
#define KEY_8       10   // 数字8
#define KEY_9       11   // 数字9
#define KEY_CONFIRM 12   // 确认键
#define KEY_STAR    13   // *键
#define KEY_0       14   // 数字0
#define KEY_HASH    15   // #键
#define KEY_NONE    16   // 预留

// 密码相关宏定义
#define PWD_LENGTH 8    // 密码长度（8位）

// 函数声明
/**
 * @brief 初始化密码输入模块（需在main中调用）
 */
void PasswordInput_Init(void);

/**
 * @brief 密码输入主循环（需在main的while中调用）
 * @return 1-验证成功，0-验证失败，2-未完成验证
 */
unsigned char PasswordInput_Loop(void);

/**
 * @brief 设置正确密码（可外部修改密码）
 * @param correctPwd 正确密码（长度必须为8位）
 */
void PasswordInput_SetCorrectPwd(unsigned char *correctPwd);

#endif