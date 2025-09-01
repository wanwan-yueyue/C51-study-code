#ifndef __SCORE_H__
#define __SCORE_H__

// 初始化积分系统
void Score_Init();

// 根据消除行数更新积分
void Score_Add(unsigned char lines);

// 获取当前积分
unsigned int Score_Get();

// 根据当前积分更新下落间隔（返回毫秒数）
unsigned int Score_GetFallInterval();

#endif