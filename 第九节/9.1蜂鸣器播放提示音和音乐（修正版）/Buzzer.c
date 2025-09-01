#include <REGX52.H>
#include <INTRINS.H>
#include "Buzzer.h"
#include "Delay.h"

// 私有变量
static unsigned int currentBPM = 120;               // 当前BPM
static unsigned char currentKeySignature = C_MAJOR; // 当前调号
static const float code SEMITONE_RATIO = 1.059463094f;     // 半音频率比 (2^(1/12))

// 调号映射表：每个调号对应的半音偏移（-1=降半音，0=不变，1=升半音）
// 顺序：C, C#, D, D#, E, F, F#, G, G#, A, A#, B
static const signed char code keySignatureMap[15][12] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},   // C_MAJOR
    {0,0,0,0,0,1,0,0,0,0,0,0},   // G_MAJOR (F#)
    {1,0,0,0,0,1,0,0,0,0,0,0},   // D_MAJOR (F#, C#)
    {1,0,0,0,0,1,0,1,0,0,0,0},   // A_MAJOR (F#, C#, G#)
    {1,0,0,0,0,1,0,1,0,1,0,0},   // E_MAJOR (F#, C#, G#, D#)
    {1,0,0,0,0,1,0,1,0,1,0,1},   // B_MAJOR (F#, C#, G#, D#, A#)
    {1,0,1,0,0,1,0,1,0,1,0,1},   // F_SHARP_MAJOR (F#, C#, G#, D#, A#, E#)
    {1,0,1,0,1,1,0,1,0,1,0,1},   // C_SHARP_MAJOR (所有音升半音)
    {0,0,0,0,0,0,1,0,0,0,0,0},   // F_MAJOR (Bb)
    {0,0,0,0,0,0,1,0,0,1,0,0},   // B_FLAT_MAJOR (Bb, Eb)
    {0,0,0,0,0,0,1,0,0,1,0,1},   // E_FLAT_MAJOR (Bb, Eb, Ab)
    {0,0,0,0,0,0,1,0,1,1,0,1},   // A_FLAT_MAJOR (Bb, Eb, Ab, Db)
    {0,0,0,0,0,0,1,0,1,1,0,1},   // D_FLAT_MAJOR (Bb, Eb, Ab, Db, Gb)
    {0,0,1,0,0,0,1,0,1,1,0,1},   // G_FLAT_MAJOR (Bb, Eb, Ab, Db, Gb, Cb)
    {0,0,1,0,1,0,1,0,1,1,0,1}    // C_FLAT_MAJOR (所有音降半音)
};

// 全局变量定义
unsigned int Buzzer_Freq = 0;
bit Buzzer_Playing = 0;

/*
// 蜂鸣器私有延时函数 (100us)
void Buzzer_Delay100us(unsigned int us)	//@11.0592MHz
{
	unsigned char i;
	while(us--)
	{
		_nop_();
		i = 43;
		while (--i);
	}
}
*/

// 初始化蜂鸣器，设置BPM和调号
void Buzzer_Init(unsigned int bpm, unsigned char keySignature)
{
    if (bpm > 0) currentBPM = bpm;
    if (keySignature <= C_FLAT_MAJOR) currentKeySignature = keySignature;
		Buzzer = 0;
}

// 修改调号
void Buzzer_SetKeySignature(unsigned char keySignature)
{
    if (keySignature <= C_FLAT_MAJOR)
    {
        currentKeySignature = keySignature;
    }
}

// 修改BPM
void Buzzer_SetBPM(unsigned int bpm)
{
    if (bpm > 0)
    {
        currentBPM = bpm;
    }
}

// 计算音符时长（毫秒）
static unsigned int CalculateNoteDuration(unsigned int noteType)
{
    unsigned int beatDuration = 60000 / currentBPM;  // 一拍的时长(毫秒)
    unsigned int duration = (4 * beatDuration) / (noteType & ~DOT);  // 基础时长
    
    if (noteType & DOT)  // 附点音符增加一半时长
    {
        duration += duration / 2;
    }
    
    return duration;
}

// 根据调号计算实际频率
static float GetActualFrequency(float baseFrequency, NoteName noteName)
{
		char i = 0;
    signed char offset = keySignatureMap[currentKeySignature][noteName];
    float actualFreq = baseFrequency;
    
    // 根据偏移计算实际频率（半音比例）
    if (offset > 0)
    {
        for (i = 0; i < offset; i++)
        {
            actualFreq *= SEMITONE_RATIO;
        }
    }
    else if (offset < 0)
    {
        for (i = 0; i > offset; i--)
        {
            actualFreq /= SEMITONE_RATIO;
        }
    }
    
    return actualFreq;
}

// 计算定时器初值
static unsigned int CalculateTimerReload(float frequency)
{
    if (frequency <= 0) return 0;  // 休止符
    // 11.0592MHz晶振，12分频，计算16位定时器初值
    return 65536 - (unsigned int)(11059200UL / (24UL * frequency));
}

// 播放指定音符
// baseFrequency: 基础频率
// noteName: 音名（用于调号处理）
// noteType: 音符类型（使用头文件中定义的宏）
void Buzzer_PlayNote(float baseFrequency, NoteName noteName, unsigned int noteType)
{
    float actualFrequency;
    unsigned int duration;
    unsigned int reload;
    
    // 参数校验（允许baseFrequency为0作为休止符，其他参数需有效）
    if (noteType == 0 || noteName < NOTE_C || noteName > NOTE_B)
    {
        Buzzer_Freq = 0;
        Buzzer_Playing = 0;
        return;
    }
    
    // 计算实际频率和时长
		
		// 处理休止符（基础频率为0）
    if (baseFrequency <= 0)
    {
        // 停止蜂鸣器，保持静音
        Buzzer_Playing = 0;
        TR0 = 0;          // 停止定时器
        Buzzer = 0;       // 确保蜂鸣器关闭
				duration = CalculateNoteDuration(noteType);
        Delay1ms(duration); // 等待休止符时长
        Delay1ms(5);      // 音符间停顿
        return;
    }
		
		 // 处理正常音符（非休止符）
    actualFrequency = GetActualFrequency(baseFrequency, noteName);
    duration = CalculateNoteDuration(noteType);
    reload = CalculateTimerReload(actualFrequency);
    
    // 设置定时器并启动
    Buzzer_Freq = reload;
    Buzzer_Playing = 1;
    TR0 = 1;  // 启动定时器0
    
    // 等待音符播放完成
    Delay1ms(duration);
    
    // 停止播放
    Buzzer_Playing = 0;
    TR0 = 0;  // 停止定时器0
    Buzzer = 0;  // 关闭蜂鸣器
    
    // 音符间停顿
    Delay1ms(5);
}