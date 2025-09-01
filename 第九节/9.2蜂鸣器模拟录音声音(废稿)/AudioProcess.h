#ifndef __AUDIOPROCESS_H__
#define __AUDIOPROCESS_H__

// 音频参数配置
#define MIN_FREQ 500    // 蜂鸣器最低频率（Hz）
#define MAX_FREQ 3000   // 蜂鸣器最高频率（Hz）
#define PCM_MAX 255     // 8位PCM最大值

// PCM数据转换为蜂鸣器频率
unsigned int PCM_To_Freq(unsigned char pcm_data);

#endif