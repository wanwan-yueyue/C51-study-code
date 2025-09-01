#include <REGX52.H>
#include "Key.h"
#include "nixie.h"
#include "Buzzer.h"
#include "delay.h"
#include "Timer0.h"


unsigned char KeyNum, i = 0;

// 定义结构体，格式：{基础频率, 音名, 音符类型}
typedef struct {
    float freq;
    NoteName name;
    unsigned int type;
} Note;

// 欢乐颂音符序列（C大调）
const Note code odeToJoy[] = {
	//(1)
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
		
    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
		
    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
		
    {e2,  NOTE_E,  DOT_QUARTER_NOTE}, // 3（附点四分音符）
    {d2,  NOTE_D,  EIGHTH_NOTE},     // 2（八分音符）
    {d2,  NOTE_D,  HALF_NOTE},      // 2（二分音符）
		
		{e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
		
	//(6)
    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
		
		{c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
		
		{d2,  NOTE_D,  DOT_QUARTER_NOTE}, // 2（附点四分音符）
    {c2,  NOTE_C,  EIGHTH_NOTE},     // 1（八分音符）
    {c2,  NOTE_C,  HALF_NOTE},      // 1（二分音符）
		
		{d2,	NOTE_D,  QUARTER_NOTE},		 // 2
		{d2,	NOTE_D,	 QUARTER_NOTE},		 // 2
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{c2, 	NOTE_C,  QUARTER_NOTE},		 // 1
		
		{d2, 	NOTE_D,	 QUARTER_NOTE},		 // 2
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3（八分音符）
		{f2,	NOTE_F,  EIGHTH_NOTE},		 // 4（八分音符）
		{e2,	NOTE_E,	 QUARTER_NOTE},		 // 3
		{c2,	NOTE_C,  QUARTER_NOTE},		 // 1
		
	//(11)
		{d2, 	NOTE_D,	 QUARTER_NOTE},		 // 2
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3（八分音符）
		{f2,	NOTE_F,  EIGHTH_NOTE},		 // 4（八分音符）
		{e2,	NOTE_E,	 QUARTER_NOTE},		 // 3
		{d2,	NOTE_D,  QUARTER_NOTE},		 // 2
		
		{c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
		{g1,  NOTE_G,  QUARTER_NOTE},    // 5(LOW1)
		{e2,	NOTE_E,  HALF_NOTE},			 // 3
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
		
		{g2,  NOTE_G,  QUARTER_NOTE},    // 5
    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
		
		{c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
		
	//(16)
		{d2,  NOTE_D,  DOT_QUARTER_NOTE}, // 2（附点四分音符）
    {c2,  NOTE_C,  EIGHTH_NOTE},     // 1（八分音符）
    {c2,  NOTE_C,  HALF_NOTE},      // 1（二分音符）
		
//		{d2,	NOTE_D,  QUARTER_NOTE},		 // 2
//		{d2,	NOTE_D,	 QUARTER_NOTE},		 // 2
//		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
//		{c2, 	NOTE_C,  QUARTER_NOTE},		 // 1
//		
//		{d2, 	NOTE_D,	 QUARTER_NOTE},		 // 2
//		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3（八分音符）
//		{f2,	NOTE_F,  EIGHTH_NOTE},		 // 4（八分音符）
//		{e2,	NOTE_E,	 QUARTER_NOTE},		 // 3
//		{c2,	NOTE_C,  QUARTER_NOTE},		 // 1
//		
//		{d2, 	NOTE_D,	 QUARTER_NOTE},		 // 2
//		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3（八分音符）
//		{f2,	NOTE_F,  EIGHTH_NOTE},		 // 4（八分音符）
//		{e2,	NOTE_E,	 QUARTER_NOTE},		 // 3
//		{d2,	NOTE_D,  QUARTER_NOTE},		 // 2
//		
//		{c2,  NOTE_C,  QUARTER_NOTE},    // 1
//    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
//		{g1,  NOTE_G,  QUARTER_NOTE},    // 5(LOW1)
//		{e2,	NOTE_E,  HALF_NOTE},			 // 3
//		
//	//(21)
//		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
//		{f2,  NOTE_F,  QUARTER_NOTE},    // 4
//    {g2,  NOTE_G,  QUARTER_NOTE},    // 5
//		
//		{g2,  NOTE_G,  QUARTER_NOTE},    // 5
//    {f2,  NOTE_F,  QUARTER_NOTE},    // 4
//    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
//    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
//		
//		{c2,  NOTE_C,  QUARTER_NOTE},    // 1
//    {c2,  NOTE_C,  QUARTER_NOTE},    // 1
//    {d2,  NOTE_D,  QUARTER_NOTE},    // 2
//    {e2,  NOTE_E,  QUARTER_NOTE},    // 3
//		
//		{d2,  NOTE_D,  DOT_QUARTER_NOTE}, // 2（附点四分音符）
//    {c2,  NOTE_C,  EIGHTH_NOTE},     // 1（八分音符）
//    {c2,  NOTE_C,  HALF_NOTE},      // 1（二分音符）
		
    {0,   NOTE_C,  0}                // 结束标志
};

//虫儿飞音符序列(F大调)
const Note code bugsFly[] = {
	//(1)
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(八分音符)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(八分音符)
		{f2,	NOTE_F,  QUARTER_NOTE},		 // 4
		{g2,	NOTE_G,  QUARTER_NOTE},		 // 5
		
		{e2,	NOTE_E,  DOT_QUARTER_NOTE},		 // 3(附点四分音符)
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(八分音符)
		{d2,	NOTE_D,  HALF_NOTE},		 // 2(二分音符)
		
		{c2,	NOTE_C,  QUARTER_NOTE},		 // 1
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(八分音符)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(八分音符)
		{d2,	NOTE_D,  QUARTER_NOTE},		 // 2
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		
		{e2,	NOTE_E,  DOT_QUARTER_NOTE},		 // 3(附点四分音符)
		{b1,	NOTE_B,  EIGHTH_NOTE},		 // 7(LOW1, 八分音符)
		{b1,	NOTE_B,  HALF_NOTE},		 // 7(LOW1, 二分音符)
		
	//(5)
		{a1,	NOTE_A,  QUARTER_NOTE},		 // 6(LOW1)
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{d2,	NOTE_D,  HALF_NOTE},		 // 2(二分音符)
		
		{a1,	NOTE_A,  QUARTER_NOTE},		 // 6(LOW1)
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{d2,	NOTE_D,  HALF_NOTE},		 // 2(二分音符)
		
		{a1,	NOTE_A,  QUARTER_NOTE},		 // 6(LOW1)
		{e2,	NOTE_E,  QUARTER_NOTE},		 // 3
		{d2,	NOTE_D,  DOT_QUARTER_NOTE},		 // 2(附点四分音符)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(八分音符)
		
		{c2,	NOTE_C,  WHOLE_NOTE},		 // 1(全音符)
			
		{0,		NOTE_C,	 0}								 // 结束标志
};


void main(){
	
	//蜂鸣器初始化,设置调号为C大调,指定bpm = 120
	Buzzer_Init(120, C_MAJOR);
	//定时器初始化
	Timer0Init();
	
	NixieDisplay(1,0);
	
	while(1){
		
		KeyNum = Key();
		if(KeyNum == 1){
				NixieDisplay(1,1);
				// 播放旋律
				Buzzer_SetBPM(96); 	//设定BPM为96
				Buzzer_SetKeySignature(C_MAJOR);	 //修改调号为C大调
				if(TR0 == 0){TR0 = 1;}	//启动定时器
        while(odeToJoy[i].freq != 0) {
            Buzzer_PlayNote(
                odeToJoy[i].freq, 
                odeToJoy[i].name, 
                odeToJoy[i].type
            );
            // 音符间短暂停顿（避免连音）
            // Delay1ms(50);
            i++;
        }
		}
		if(KeyNum == 2){
				NixieDisplay(1,2);
				// 播放旋律
				Buzzer_SetBPM(103); 	//设定BPM为103
				Buzzer_SetKeySignature(F_MAJOR);	 //修改调号为F大调
				if(TR0 == 0){TR0 = 1;}	//启动定时器
        while(bugsFly[i].freq != 0) {
            Buzzer_PlayNote(
                bugsFly[i].freq, 
                bugsFly[i].name, 
                bugsFly[i].type
            );
            // 音符间短暂停顿（避免连音）
            // Delay1ms(50);
            i++;
        }
		}
		if(KeyNum == 4){
				NixieDisplay(1,4);
				//重新开始
				if(TR0 == 1){TR0 = 0;}	//关闭定时器
        i = 0;
		}
		
	}
	
}