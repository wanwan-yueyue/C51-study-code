#include <REGX52.H>
#include "delay.h"
#include "password_input.h"

void main() {
    unsigned char Result;
    // 初始化密码输入模块
    PasswordInput_Init();

    // （可选）设置自定义正确密码（默认是"12345678"）
    // unsigned char MyCorrectPwd[9] = "";
    // PasswordInput_SetCorrectPwd(MyCorrectPwd);

    while (1) {
        // 运行密码输入主循环，获取结果
        Result = PasswordInput_Loop();
        
        // 可根据验证结果扩展其他功能（示例）
        if (Result == 1) {
            // 密码正确：执行开门、解锁等操作
            // 例如：Relay_Open();
        } else if (Result == 0) {
            // 密码错误：执行报警、记录等操作
            // 例如：Buzzer_Beep(1);
        }
    }
}