#include <REGX52.H>
#include "LCD1602.h"    // LCD1602显示屏驱动库
#include "DS1302.h"     // DS1302实时时钟芯片驱动库
#include "Key.h"        // 按键扫描驱动库
#include "Timer0.h"     // 定时器0驱动库

// 全局变量定义
unsigned char KeyNum;           // 存储按键编号
unsigned char MODE = 0;         // 工作模式：0-显示模式（正常显示时间），1-设置模式（可调整时间）
unsigned char TimeSetSelect = 0;// 设置模式下选中的时间单位（0-年，1-月，2-日，3-时，4-分，5-秒）
bit TimeSetFlashFlag = 0; // 闪烁标志（0-不闪烁，1-闪烁，用于设置模式下选中项的闪烁效果）


// 时间单位在LCD上的显示位置（行,列）
// 对应顺序：[0]年，[1]月，[2]日，[3]时，[4]分，[5]秒
unsigned char code TimePos[6][2] = {{1,1}, {1,4}, {1,7}, {2,1}, {2,4}, {2,7}};

// 上电默认时间设置（每次上电后强制初始化的时间）
// 格式：{年, 月, 日, 时, 分, 秒}，年份为20xx年后两位（如23表示2023年）
unsigned char code DEFAULT_TIME[6] = {25, 8, 2, 0, 0, 0}; // 默认2025年8月2日 00:00:00


/**
 * @brief   判断是否为闰年（针对20xx年，简化判断：能被4整除即为闰年）
 * @param   year: 年份（20xx年后两位，如23表示2023年）
 * @retval  1-是闰年，0-不是闰年
 */
bit isLeapYear(unsigned char year) {
    return (year % 4 == 0);  // 能被4整除即为闰年
}


/**
 * @brief   获取当月的最大天数（根据年份和月份判断）
 * @param   year: 年份（20xx年后两位）
 * @param   month: 月份（1-12）
 * @retval  当月的最大天数（28/29/30/31）
 */
unsigned char getMaxDay(unsigned char year, unsigned char month) {
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;  // 大月（1/3/5/7/8/10/12月有31天）
        case 4: case 6: case 9: case 11:
            return 30;  // 小月（4/6/9/11月有30天）
        case 2:
            // 2月：闰年29天，平年28天
            return isLeapYear(year) ? 29 : 28;
        default:
            return 31;  // 月份不合法时默认31天（容错处理）
    }
}


/**
 * @brief   更新时间显示（处理设置模式下选中项的闪烁效果）
 * @note    遍历所有时间单位，根据模式和闪烁标志决定显示数字或空白（实现闪烁）
 */
void UpdateTimeDisplay() {
    unsigned char i;
    for(i = 0; i < 6; i++) {  // 遍历6个时间单位（年到秒）
        // 若当前为设置模式，且是选中项，且闪烁标志为1，则显示空白（实现闪烁）
        if((i == TimeSetSelect) && (TimeSetFlashFlag != 0) && MODE == 1) {
            LCD_ShowString(TimePos[i][0], TimePos[i][1], "  "); 
        } else {
            // 否则显示对应时间值（2位数字，不足补0）
            LCD_ShowNum(TimePos[i][0], TimePos[i][1], DS1302_Time[i], 2);
        }
    }
}


/**
 * @brief   时间显示函数（显示模式下调用）
 * @note    从DS1302读取当前时间，然后调用UpdateTimeDisplay刷新LCD显示
 */
void TimeShow(void) {
    DS1302_ReadTime();  // 从DS1302芯片读取最新时间到DS1302_Time数组
    UpdateTimeDisplay();  // 刷新LCD显示（无闪烁）
}


/**
 * @brief   调整时间的通用函数（设置模式下调整选中的时间单位）
 * @param   step: 调整步长（+1-增加，-1-减少）
 * @note    根据选中的时间单位（年/月/日/时/分/秒）处理边界值（如小时最大23）
 */
void adjustTime(char step) {
    unsigned char maxDay;  // 用于存储当月最大天数
    
    // 调整选中的时间单位
    DS1302_Time[TimeSetSelect] = DS1302_Time[TimeSetSelect] + step;
    
    // 根据选中的时间单位处理边界值
    switch(TimeSetSelect) {
        case 0:  // 调整年份
            if(DS1302_Time[0] > 99) DS1302_Time[0] = 0;  
            if(DS1302_Time[0] < 0) DS1302_Time[0] = 99;  
            // 年份变化可能影响2月天数，需检查日期是否合法
            if(DS1302_Time[2] > getMaxDay(DS1302_Time[0], DS1302_Time[1])) {
                DS1302_Time[2] = getMaxDay(DS1302_Time[0], DS1302_Time[1]);
            }
            break;
            
        case 1:  // 调整月份
            if(DS1302_Time[1] > 12) DS1302_Time[1] = 1;  
            if(DS1302_Time[1] < 1) DS1302_Time[1] = 12;  
            // 月份变化可能影响当月天数，需检查日期是否合法
            if(DS1302_Time[2] > getMaxDay(DS1302_Time[0], DS1302_Time[1])) {
                DS1302_Time[2] = getMaxDay(DS1302_Time[0], DS1302_Time[1]);
            }
            break;
            
        case 2:  // 调整日期
            maxDay = getMaxDay(DS1302_Time[0], DS1302_Time[1]);  
            if(DS1302_Time[2] > maxDay) DS1302_Time[2] = 1;     
            if(DS1302_Time[2] < 1) DS1302_Time[2] = maxDay;     
            break;
            
        case 3:  // 调整小时
            if(DS1302_Time[3] > 23) DS1302_Time[3] = 0; 
            if(DS1302_Time[3] < 0) DS1302_Time[3] = 23;  
            break;
            
        case 4:  // 调整分钟
            if(DS1302_Time[4] > 59) DS1302_Time[4] = 0; 
            if(DS1302_Time[4] < 0) DS1302_Time[4] = 59;  
            break;
            
        case 5:  // 调整秒钟
            if(DS1302_Time[5] > 59) DS1302_Time[5] = 0; 
            if(DS1302_Time[5] < 0) DS1302_Time[5] = 59;  
            break;
    }
    
    UpdateTimeDisplay();  // 调整后立即刷新显示（实时反馈调整结果）
}


/**
 * @brief   时间设置函数（设置模式下调用，处理按键逻辑）
 * @note    根据按键编号（K2/K3/K4）执行对应操作：切换选中项/增加/减少
 */
void TimeSet(void) {
    if(KeyNum == 2) {  // K2按键：切换选中的时间单位（年→月→日→时→分→秒循环）
        if(++TimeSetSelect>=6) TimeSetSelect=0;
    } 
    else if(KeyNum == 3) {  // K3按键：增加当前选中的时间单位
        adjustTime(1);  // 调用调整函数，步长+1
    } 
    else if(KeyNum == 4) {  // K4按键：减少当前选中的时间单位
        adjustTime(-1);  // 调用调整函数，步长-1
    }
}


/**
 * @brief   主函数（程序入口）
 * @note    初始化硬件→设置上电默认时间→进入主循环（处理按键和模式切换）
 */
void main(){
    unsigned char i;  // 循环变量（用于初始化默认时间）
    
    // 硬件初始化
    LCD_Init();        // 初始化LCD1602显示屏
    DS1302_Init();     // 初始化DS1302实时时钟芯片
    Timer0Init();      // 初始化定时器0（用于控制闪烁频率）
    EA = 1;            // 使能总中断（确保定时器0中断能正常触发）
    
    // 初始化LCD上的分隔符（格式化显示格式）
    LCD_ShowString(1,3,"-");  
    LCD_ShowString(1,6,"-");  
    LCD_ShowString(2,3,":");  
    LCD_ShowString(2,6,":");  
    
    // 上电强制设置默认时间（核心逻辑：每次上电覆盖DS1302原有数据）
    for(i = 0; i < 6; i++) {
        DS1302_Time[i] = DEFAULT_TIME[i];  // 将默认时间复制到时间数组
    }
    DS1302_SetTime();  // 将时间数组中的默认时间写入DS1302芯片（永久保存）
    
    // 主循环
    while(1){
        KeyNum = Key();  // 读取按键状态
        
        // K1按键：切换工作模式（显示模式↔设置模式）
        if(KeyNum == 1) {
            if(MODE == 0) {  // 当前是显示模式→切换到设置模式
                MODE = 1;                // 模式切换为设置模式
                TimeSetSelect = 0;       // 初始选中"年"
                DS1302_ReadTime();       // 从DS1302读取当前时间到数组（作为调整基准）
            } else {  // 当前是设置模式→切换到显示模式
                MODE = 0;                // 模式切换为显示模式
                DS1302_SetTime();        // 将调整后的时间写入DS1302（保存设置结果）
            }
        }
        
        // 根据当前模式执行对应功能
        switch(MODE) {
            case 0:  // 显示模式：正常显示时间（从DS1302实时读取）
                TimeShow();
                break;
            case 1:  // 设置模式：处理按键调整，并刷新显示（带闪烁效果）
                TimeSet();          // 处理按键（切换选中项/增减）
                UpdateTimeDisplay();// 刷新显示（含闪烁效果）
                break;
        }
        
        KeyNum = 0;  // 清除按键状态（避免一次按键被重复处理）
    }
}


/**
 * @brief   定时器0中断服务函数（用于控制设置模式下的闪烁频率）
 * @note    每500ms翻转一次闪烁标志，使选中项交替显示数字和空白（实现闪烁）
 */
void Timer0_Routine() interrupt 1{
    static unsigned int T0Count = 0;  // 静态变量，用于计数（记录中断次数）
    
    // 重装载定时器0初值（确保每次中断间隔1ms，因初始化时已配置为1ms中断一次）
    TL0 = 0x66;  
    TH0 = 0xFC;
    
    // 每500ms翻转一次闪烁标志（1ms×500=500ms）
    if(++T0Count >= 500) {  
        T0Count = 0;                // 计数清零
        TimeSetFlashFlag = !TimeSetFlashFlag;  // 翻转闪烁标志（0→1或1→0）
    }
}