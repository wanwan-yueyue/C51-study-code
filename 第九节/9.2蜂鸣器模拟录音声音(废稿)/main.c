#include <REGX52.H>
#include "Timer0.h"
#include "Uart.h"
#include "Buzzer.h"

void main() {
    Timer0Init();    // 初始化定时器0（蜂鸣器驱动）
    Uart_Init();     // 初始化串口（9600bps）
    Buzzer_Init(120, C_MAJOR);  // 初始化蜂鸣器（BPM和调号不影响实时模式）
    
    while(1) {
        // 主循环无需操作，依赖中断实时处理
    }
}