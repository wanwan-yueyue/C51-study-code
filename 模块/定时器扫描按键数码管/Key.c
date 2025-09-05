#include <REGX52.H>
#include "Delay.h"

unsigned char Key_KeyNumber;

/**
  * @brief  获取按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,0表示无按键按下
  */
unsigned char Key(void)
{
    unsigned char Temp = 0;
    Temp = Key_KeyNumber;
    Key_KeyNumber = 0;  //读取后清零，避免重复触发
    return Temp;
}

/**
  * @brief  获取当前按键的状态，带简单消抖
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,0表示无按键按下
  */
unsigned char Key_GetState()
{
    unsigned char KeyNumber = 0;
    
    //先检测是否有按键按下
    if(P3_1 == 0 || P3_0 == 0 || P3_2 == 0 || P3_3 == 0)
    {
        Delay1ms(10);  //10ms消抖
        //再次检测确认
        if(P3_1 == 0){KeyNumber = 1;}
        if(P3_0 == 0){KeyNumber = 2;}
        if(P3_2 == 0){KeyNumber = 3;}
        if(P3_3 == 0){KeyNumber = 4;}
    }
    return KeyNumber;
}

/**
  * @brief  按键驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void Key_Loop(void)
{
    static unsigned char NowState, LastState;
    LastState = NowState;              //保存上一次状态
    NowState = Key_GetState();         //获取当前状态
    
    //松手检测：上一状态按下，当前状态未按下，视为有效按键
    if(LastState != 0 && NowState == 0)
    {
        Key_KeyNumber = LastState;  //记录键码
    }
}