#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"
#include "AT24C02.h"

unsigned char KeyNum;
unsigned char Min, Sec, MiniSec;
unsigned char RunFlag;

void main()
{
    Timer0Init();
    while(1)
    {
        KeyNum = Key();
        if(KeyNum == 1)            //K1按键按下：启停切换
        {
            RunFlag = !RunFlag;
        }
        if(KeyNum == 2)            //K2按键按下：清零
        {
            Min = 0;
            Sec = 0;
            MiniSec = 0;
        }
        if(KeyNum == 3)            //K3按键按下：保存数据
        {
            AT24C02_WriteByte(0, Min);
            Delay1ms(5);
            AT24C02_WriteByte(1, Sec);
            Delay1ms(5);
            AT24C02_WriteByte(2, MiniSec);
            Delay1ms(5);
        }
        if(KeyNum == 4)            //K4按键按下：读取数据
        {
            Min = AT24C02_ReadByte(0);
            Sec = AT24C02_ReadByte(1);
            MiniSec = AT24C02_ReadByte(2);
            // 读取后增加范围修正，避免超出合理范围
            Min %= 60;    // 分钟最大59
            Sec %= 60;    // 秒钟最大59
            MiniSec %= 100;// 毫秒位最大99
        }
        
        // 数码管显示：增加数值范围检查，避免段码索引越界
        Nixie_SetBuf(1, (Min/10) % 10);    // 分钟十位（0-5）
        Nixie_SetBuf(2, Min%10);           // 分钟个位（0-9）
        Nixie_SetBuf(3, 11);               // 分隔符（段码表11）
        Nixie_SetBuf(4, (Sec/10) % 10);    // 秒钟十位（0-5）
        Nixie_SetBuf(5, Sec%10);           // 秒钟个位（0-9）
        Nixie_SetBuf(6, 11);               // 分隔符
        Nixie_SetBuf(7, (MiniSec/10) % 10);// 毫秒十位（0-9）
        Nixie_SetBuf(8, MiniSec%10);       // 毫秒个位（0-9）
    }
}

/**
  * @brief  秒表驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void Sec_Loop(void)
{
    if(RunFlag)
    {
        MiniSec++;
        if(MiniSec >= 100)    // 100个10ms = 1秒
        {
            MiniSec = 0;
            Sec++;
            if(Sec >= 60)
            {
                Sec = 0;
                Min++;
                if(Min >= 60)    // 分钟最大59
                {
                    Min = 0;
                }
            }
        }
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0Count1, T0Count2, T0Count3;
    TL0 = 0x18;        //设置定时初值（1ms@12MHz）
    TH0 = 0xFC;
    T0Count1++;
    if(T0Count1 >= 20)    // 20ms扫描一次按键
    {
        T0Count1 = 0;
        Key_Loop();
    }
    T0Count2++;
    if(T0Count2 >= 2)    // 2ms扫描一次数码管（刷新频率更高，显示更稳定）
    {
        T0Count2 = 0;
        Nixie_Loop();
    }
    T0Count3++;
    if(T0Count3 >= 10)    // 10ms更新一次秒表计数
    {
        T0Count3 = 0;
        Sec_Loop();
    }
}