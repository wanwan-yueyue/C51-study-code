#include "score.h"
#include <REGX52.H>

// 模块内部变量（私有）
static unsigned int score = 0;          // 积分值
static unsigned int fallInterval = 1000; // 初始下落间隔1000ms


/**
 * @brief 初始化积分系统
 */
void Score_Init() {
    score = 0;
    fallInterval = 1000;
}

/**
 * @brief 根据消除行数增加积分
 * @param lines 消除的行数（1-3）
 */
void Score_Add(unsigned char lines) {
    // 积分规则：多消行奖励递增
    switch(lines) {
        case 1: score += 1; break;
        case 2: score += 3; break;
        case 3: score += 6; break;
    }
    // 限制最大积分（防止溢出）
    if(score > 9999) score = 9999;
    // 更新难度（下落速度）
    Score_GetFallInterval();
}

///**
// * @brief 获取当前积分
// */
//unsigned int Score_Get() {
//    return score;
//}


/**
 * @brief 根据积分计算下落间隔（难度曲线）
 * @return 下落间隔（ms），最低200ms
 */
unsigned int Score_GetFallInterval() {
    // 每增加10分，速度提升100ms（原初始间隔1000ms保持不变）
    unsigned int newInterval = 1000 - (score / 10) * 100;
    // 限制最低间隔为200ms，保持原样
    fallInterval = (newInterval < 200) ? 200 : newInterval;
    return fallInterval;
}
    