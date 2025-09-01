#include "AudioProcess.h"

// 将8位PCM数据（0~255）映射到500~3000Hz
unsigned int PCM_To_Freq(unsigned char pcm_data) {
    // 线性映射公式：freq = MIN_FREQ + (pcm_data * (MAX_FREQ - MIN_FREQ) / PCM_MAX)
    return MIN_FREQ + (pcm_data * (MAX_FREQ - MIN_FREQ)) / PCM_MAX;
}