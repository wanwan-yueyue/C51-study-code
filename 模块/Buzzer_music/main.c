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

// 答案音符序列（C大调）
const Note code answer[] = {
	//有个简单的问题，
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  SIXTEENTH_NOTE},		 // 1(HIGH1)
		{g1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{g1,	NOTE_G,  HALF_NOTE},		 // 5
		
	//什么是爱情？
		{g1,	NOTE_G,  EIGHTH_NOTE},		 // 5
		{g1,	NOTE_G,  EIGHTH_NOTE},		 // 5
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{e1,	NOTE_E,  DOT_EIGHTH_NOTE},		 // 3
		{c1,  NOTE_C,  QUARTER_NOTE},			// 1
		{0,   NOTE_C,  QUARTER_NOTE},			// 0
		
	//它是否是一种味道还是引力？
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  SIXTEENTH_NOTE},		 // 1(HIGH1)
		{a1, 	NOTE_A,  DOT_EIGHTH_NOTE},     // 6
		{g1,	NOTE_G,  EIGHTH_NOTE},		 // 5
		{g1,	NOTE_G,  EIGHTH_NOTE},		 // 5
		
		{a1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{g1,	NOTE_G,  EIGHTH_NOTE},		 // 5
		{a1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{e1,	NOTE_E,  DOT_QUARTER_NOTE},     // 3
		{0,   NOTE_C,  EIGHTH_NOTE},			// 0
		
	//从我初恋那天起，
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{c2,	NOTE_C,  SIXTEENTH_NOTE},		 // 1(HIGH1)
		{g1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{g1,	NOTE_G,  HALF_NOTE},		 // 5
		{0,   NOTE_C,  QUARTER_NOTE},			// 0
		
	//先是甜蜜然后紧接就会有风雨，
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{c2,	NOTE_C,  DOT_QUARTER_NOTE},		 // 1(HIGH1)
		
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  SIXTEENTH_NOTE},		 // 1(HIGH1)
		{g1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{g1,	NOTE_G,  QUARTER_NOTE},		 // 5
		{g1, 	NOTE_A,  EIGHTH_NOTE},     // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  DOT_HALF_NOTE},		 // 1(HIGH1)
	
	//爱就像蓝天白云晴空万里
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
	//突然暴风雨
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{g2,	NOTE_G,  QUARTER_NOTE},		 // 5(HIGH1)
		{e2,	NOTE_E,  DOT_QUARTER_NOTE},		 // 3(HIGH1)
		
	//无处躲避总是让人始料不及
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{c2,	NOTE_C,  DOT_QUARTER_NOTE},		 // 1(HIGH1)
		
	//人就像患重感冒打着喷嚏
		{g1,	NOTE_G,  SIXTEENTH_NOTE},		 // 5
		{a1,	NOTE_A,  SIXTEENTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
	//发烧要休息
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{g2,	NOTE_G,  QUARTER_NOTE},		 // 5(HIGH1)
		{e2,	NOTE_E,  DOT_QUARTER_NOTE},		 // 3(HIGH1)
		
	//冷热交替欢喜犹豫乐此不疲
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  SIXTEENTH_NOTE},		 // 2(HIGH1)
		{e2,	NOTE_E,  EIGHTH_NOTE},		 // 3(HIGH1)
		{a1,	NOTE_A,  DOT_EIGHTH_NOTE},		 // 6
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{d2,	NOTE_D,  EIGHTH_NOTE},		 // 2(HIGH1)
		{c2,	NOTE_C,  EIGHTH_NOTE},		 // 1(HIGH1)
		{c2,	NOTE_C,  HALF_NOTE},		 // 1(HIGH1)
			
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
				Buzzer_SetBPM(74); 	//设定BPM为74
				Buzzer_SetKeySignature(C_MAJOR);	 //修改调号为C大调
				if(TR0 == 0){TR0 = 1;}	//启动定时器
        while(answer[i].freq != 0 || answer[i].type != 0) {
            Buzzer_PlayNote(
                answer[i].freq, 
                answer[i].name, 
                answer[i].type
            );
            // 音符间短暂停顿（避免连音）
            // Delay1ms(50);
            i++;
        }
		}
//		if(KeyNum == 2){
//				NixieDisplay(1,2);
//				// 播放旋律
//				Buzzer_SetBPM(103); 	//设定BPM为103
//				Buzzer_SetKeySignature(F_MAJOR);	 //修改调号为F大调
//				if(TR0 == 0){TR0 = 1;}	//启动定时器
//        while(bugsFly[i].freq != 0) {
//            Buzzer_PlayNote(
//                bugsFly[i].freq, 
//                bugsFly[i].name, 
//                bugsFly[i].type
//            );
//            // 音符间短暂停顿（避免连音）
//            // Delay1ms(50);
//            i++;
//        }
//		}
		if(KeyNum == 4){
				NixieDisplay(1,4);
				//重新开始
				if(TR0 == 1){TR0 = 0;}	//关闭定时器
        i = 0;
		}
		
	}
	
}