#ifndef __BUZZER__H_
#define __BUZZER__H_

#include <REGX52.H>
#include "Timer0.h"  // 引入定时器头文件

// 调号定义
#define C_MAJOR         0   // C大调/ a小调
#define G_MAJOR         1   // G大调/ e小调
#define D_MAJOR         2   // D大调/ b小调
#define A_MAJOR         3   // A大调/ f#小调
#define E_MAJOR         4   // E大调/ c#小调
#define B_MAJOR         5   // B大调/ g#小调
#define F_SHARP_MAJOR   6   // F#大调/ d#小调
#define C_SHARP_MAJOR   7   // C#大调/ a#小调
#define F_MAJOR         8   // F大调/ d小调
#define B_FLAT_MAJOR    9   // 降B大调/ g小调
#define E_FLAT_MAJOR    10  // 降E大调/ c小调
#define A_FLAT_MAJOR    11  // 降A大调/ f小调
#define D_FLAT_MAJOR    12  // 降D大调/ b_flat小调
#define G_FLAT_MAJOR    13  // 降G大调/ e_flat小调
#define C_FLAT_MAJOR    14  // 降C大调/ a_flat小调

// 音符类型定义
#define DOT             0x80    // 附点标记（用于按位或）
#define WHOLE_NOTE      1       // 全音符（4拍）
#define HALF_NOTE       2       // 二分音符（2拍）
#define QUARTER_NOTE    4       // 四分音符（1拍）
#define EIGHTH_NOTE     8       // 八分音符（1/2拍）
#define SIXTEENTH_NOTE  16      // 十六分音符（1/4拍）

// 附点音符（原时长 + 1/2原时长）
#define DOT_WHOLE_NOTE      (WHOLE_NOTE | DOT)
#define DOT_HALF_NOTE       (HALF_NOTE | DOT)
#define DOT_QUARTER_NOTE    (QUARTER_NOTE | DOT)
#define DOT_EIGHTH_NOTE     (EIGHTH_NOTE | DOT)
#define DOT_SIXTEENTH_NOTE  (SIXTEENTH_NOTE | DOT)

// 音名枚举（用于调号映射）
typedef enum {
    NOTE_C, NOTE_C_SHARP, NOTE_D, NOTE_D_SHARP, NOTE_E, NOTE_F,
    NOTE_F_SHARP, NOTE_G, NOTE_G_SHARP, NOTE_A, NOTE_A_SHARP, NOTE_B
} NoteName;

// 音高宏定义（根据音高与频率对照表）
#define A2 27.5
#define Asharp2_Bflat2 29.14
#define B2 30.87
#define C1 32.7
#define Csharp1_Dflat1 34.65
#define D1 36.71
#define Dsharp1_Eflat1 38.89
#define E1 41.2
#define F1 43.65
#define Fsharp1_Gflat1 46.25
#define G1 49.0
#define Gsharp1_Aflat1 51.91
#define A1 55.0
#define Asharp1_Bflat1 58.27
#define B1 61.74

#define C 65.41
#define Csharp_Dflat 69.3
#define D 73.42
#define Dsharp_Eflat 77.78
#define E 82.41
#define F 87.31
#define Fsharp_Gflat 92.5
#define G 98.0
#define Gsharp_Aflat 103.83
#define A 110.0
#define Asharp_Bflat 116.54
#define B 123.47

#define c 130.81
#define csharp_db 138.59
#define d 146.83
#define dsharp_eb 155.56
#define e 164.81
#define f 174.61
#define fsharp_gb 185.0
#define g 196.0
#define gsharp_ab 207.65
#define a 220.0
#define asharp_bb 233.08
#define b 246.94

#define c1 261.63    // 中央C
#define csharp1_db1 277.18
#define d1 293.66
#define dsharp1_eb1 311.13
#define e1 329.63
#define f1 349.23
#define fsharp1_gb1 369.99
#define g1 392.0
#define gsharp1_ab1 415.3
#define a1 440.0          // A4标准音
#define asharp1_bb1 466.16
#define b1 493.88

#define c2 523.25
#define csharp2_db2 554.37
#define d2 587.33
#define dsharp2_eb2 622.25
#define e2 659.25
#define f2 698.46
#define fsharp2_gb2 739.99
#define g2 783.99
#define gsharp2_ab2 830.61
#define a2 880.0
#define asharp2_bb2 932.33
#define b2 987.77

#define c3 1046.5
#define csharp3_db3 1108.73
#define d3 1174.66
#define dsharp3_eb3 1244.51
#define e3 1318.51
#define f3 1396.91
#define fsharp3_gb3 1479.98
#define g3 1567.98
#define gsharp3_ab3 1661.22
#define a3 1760.0
#define asharp3_bb3 1864.66
#define b3 1975.53

#define c4 2093.0
#define csharp4_db4 2217.46
#define d4 2349.32
#define dsharp4_eb4 2489.02
#define e4 2637.02
#define f4 2793.83
#define fsharp4_gb4 2959.96
#define g4 3135.96
#define gsharp4_ab4 3322.44
#define a4 3520.0
#define asharp4_bb4 3729.31
#define b4 3951.07

#define c5 4186.01

// 蜂鸣器端口
sbit Buzzer = P2^5;

// 定时器相关全局变量
extern unsigned int Buzzer_Freq;  // 存储定时器初值
extern bit Buzzer_Playing;        // 播放状态标志

// 函数声明
void Buzzer_Init(unsigned int bpm, unsigned char keySignature);
void Buzzer_SetKeySignature(unsigned char keySignature);
void Buzzer_SetBPM(unsigned int bpm);
void Buzzer_PlayNote(float baseFrequency, NoteName noteName, unsigned int noteType);

#endif

/*
// 定义结构体，格式：{基础频率, 音名, 音符类型}
typedef struct {
    float freq;
    NoteName name;
    unsigned int type;
} Note;

// 音符序列
const Note code odeToJoy[] = {
    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
}
*/
