#include "password_input.h"
#include "delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

// 内部全局变量（仅模块内可见）
static unsigned char KeyNum = 0;                  // 按键值
static unsigned char InputPassword[PWD_LENGTH + 1] = {0};  // 输入密码数组
static unsigned char InputCount = 0;              // 输入长度计数
static unsigned char code DefaultCorrectPwd[PWD_LENGTH + 1] = "12345678";  // 默认正确密码
static unsigned char *CorrectPwd = DefaultCorrectPwd;  // 当前正确密码

// 内部函数声明（仅模块内使用）
static void UpdatePasswordDisplay(void);  // 更新显示
static bit CheckPassword(void);           // 验证密码
static void ClearInput(void);             // 清空输入

/**
 * 初始化密码输入模块
 */
void PasswordInput_Init(void) {
    LCD_Init();  // 初始化LCD
    LCD_ShowString(1, 1, "Password:");    // 显示提示
    LCD_ShowString(2, 1, "                ");  // 清空第二行
}

/**
 * 更新密码显示（内部函数）
 */
static void UpdatePasswordDisplay(void) {
    unsigned char i;
    // 清空第二行显示区域
    LCD_ShowString(2, 1, "                ");
    // 显示已输入的密码字符
    for (i = 0; i < InputCount; i++) {
        LCD_ShowChar(2, i + 1, InputPassword[i]);
    }
}

/**
 * 验证密码（内部函数）
 * @return 1-正确，0-错误
 */
static bit CheckPassword(void) {
    unsigned char i;
    for (i = 0; i < PWD_LENGTH; i++) {
        if (InputPassword[i] != CorrectPwd[i]) {
            return 0;  // 有一位不同则错误
        }
    }
    return 1;  // 全部相同则正确
}

/**
 * 清空输入（内部函数）
 */
static void ClearInput(void) {
    unsigned char i;
    for (i = 0; i < PWD_LENGTH; i++) {
        InputPassword[i] = 0;
    }
    InputCount = 0;
    UpdatePasswordDisplay();
}

/**
 * 密码输入主循环
 * @return 1-验证成功，0-验证失败，2-未完成验证
 */
unsigned char PasswordInput_Loop(void) {
    KeyNum = MatrixKey();  // 读取按键

    if (KeyNum != 0) {  // 有按键按下
        // 处理数字键1-3（S1-S3）
        if (KeyNum >= KEY_1 && KeyNum <= KEY_3) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '0' + KeyNum;  // 转换为'1'-'3'
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理数字键4-6（S5-S7）
        else if (KeyNum >= KEY_4 && KeyNum <= KEY_6) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '0' + (KeyNum - 1);  // 转换为'4'-'6'
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理数字键7-9（S9-S11）
        else if (KeyNum >= KEY_7 && KeyNum <= KEY_9) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '0' + (KeyNum - 2);  // 转换为'7'-'9'
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理0键（S14）
        else if (KeyNum == KEY_0) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '0';
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理*键（S13）
        else if (KeyNum == KEY_STAR) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '*';
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理#键（S15）
        else if (KeyNum == KEY_HASH) {
            if (InputCount < PWD_LENGTH) {
                InputPassword[InputCount] = '#';
                InputCount++;
                UpdatePasswordDisplay();
            }
        }
        // 处理退格键（S4）
        else if (KeyNum == KEY_BACKSPACE) {
            if (InputCount > 0) {
                InputCount--;
                InputPassword[InputCount] = 0;  // 清除最后一位
                UpdatePasswordDisplay();
            }
        }
        // 处理清除键（S8）
        else if (KeyNum == KEY_CLEAR) {
            ClearInput();  // 清空输入
        }
        // 处理确认键（S12）
        else if (KeyNum == KEY_CONFIRM) {
            if (InputCount == PWD_LENGTH) {  // 输入满8位才验证
                if (CheckPassword()) {
                    LCD_ShowString(2, 1, "Correct!      ");
                    Delay1ms(1500);  // 显示结果
                    ClearInput();    // 重置输入
                    return 1;  // 返回验证成功
                } else {
                    LCD_ShowString(2, 1, "Wrong!        ");
                    Delay1ms(1500);
                    ClearInput();
                    return 0;  // 返回验证失败
                }
            } else {
                LCD_ShowString(2, 1, "Need 8 chars! ");  // 提示输入不足
                Delay1ms(1000);
                UpdatePasswordDisplay();  // 恢复显示输入内容
            }
        }
    }
    return 2;  // 未完成验证
}

/**
 * 设置自定义正确密码
 * @param correctPwd 指向8位密码的指针
 */
void PasswordInput_SetCorrectPwd(unsigned char *correctPwd) {
    if (correctPwd != 0) {  // 指针非空才设置
        CorrectPwd = correctPwd;
    }
}