#include <REGX52.H>

//列选信号数组，依次选中第1-4列
static unsigned char idata ColPins[] = {0x08, 0x04, 0x02, 0x01};
//按键值映射表
static unsigned char idata KeyMap[4][4] = {
    {1 ,  2,  3,  4},
    {5 ,  6,  7,  8},
    {9 , 10, 11, 12},
    {13, 14, 15, 16}
};

static unsigned char idata keyState[4][4] = {0};  				//按键状态 0-未按下 1-按下
static unsigned char idata keyDebounce[4][4] = {0};				//消抖计数
static unsigned char currentKey = 0;       					//当前按键值
static unsigned char scanCol = 0;         					//当前扫描列


/**
 * @brief  定时器中断中调用的键盘扫描函数
 * @param  无
 * @retval 无
 */
void NonBlockKey_Scan(void) {
    unsigned char row, rowVal;
    
    //先将所有列置高，再将当前列置低
    P1 = 0xFF;
    P1 &= ~ColPins[scanCol];
    
    //读取行状态
    rowVal = (P1 >> 4) & 0x0F; // 取P1_7-P1_4的值
    
    //检测每一行
    for(row = 0; row < 4; row++) {
        if(!(rowVal & (1 << (3 - row)))) {
            //按键按下，增加消抖计数
            if(keyDebounce[row][scanCol] < 10) {
                keyDebounce[row][scanCol]++;
                //消抖计数达到阈值，确认按键按下
                if(keyDebounce[row][scanCol] == 10) {
                    keyState[row][scanCol] = 1;
                    currentKey = KeyMap[row][scanCol];
                }
            }
        } else {
            //按键未按下，重置消抖计数和状态
            if(keyDebounce[row][scanCol] > 0) {
                keyDebounce[row][scanCol]--;
                if(keyDebounce[row][scanCol] == 0) {
                    keyState[row][scanCol] = 0;
                }
            }
        }
    }
    
    //切换到下一列
    scanCol = (scanCol + 1) % 4;
}

/**
 * @brief  获取当前按键值（非阻塞，仅返回一次有效按键）
 * @param  无
 * @retval 按键值（1~16，无按键时返回 0）
 */
unsigned char NonBlockKey_GetKey(void) {
    unsigned char temp = currentKey;
    currentKey = 0;  // 读取后清除，确保只获取一次
    return temp;
}