/*
 * Kioku_test_program.h
 *
 *  Created on: Jan 23, 2024
 *      Author: yuki(denshi-1996)
 */
#include "CH32V003F4P6_Mylib.h"

#ifndef USER_KIOKU_GAME_PROGRAM_H_
#define USER_KIOKU_GAME_PROGRAM_H_

#define LED_RED PD0
#define LED_GREEN PA2
#define LED_BLUE PA1

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4

#define SW_RED PD4
#define SW_GREEN PD5
#define SW_BLUE PD6
#define SW_YELLOW PD7

#define SEG_DOT 0x80

#define MODE_START 0
#define MODE_GAME_START 5
#define MODE_RANDOM 10
#define MODE_LED_BEEP 17
#define MODE_WAIT 15
#define MODE_JUDGE 20
#define MODE_CLEAR_ONCE 25
#define MODE_CLEAR 30
#define MODE_GAMEOVER 100
#define MODE_GAMEOVER_RUN 110
#define MODE_TIMER_SELECT 500
#define MODE_TIMER_SECTION 530
#define MODE_TIMER_END 550

#define HISCORE_FLASH 0x08003FC0
#define DIF_TIME_FLASH 0x08003FC2

#define SEG_SEQUENCE_WAIT_TIME 600
#define SEG_SEQUENCE_1DEG_WAIT_TIME 400

//#define RED_BEEP SI4
//#define GREEN_BEEP SO4
//#define BLUE_BEEP RA4
//#define YELLOW_BEEP RE4
#define RED_BEEP DO4
#define GREEN_BEEP RE4
#define BLUE_BEEP MI4
#define YELLOW_BEEP FA4
//#define RED_BEEP DO4
//#define GREEN_BEEP DO4
//#define BLUE_BEEP DO4
//#define YELLOW_BEEP DO4

#define GAMEOVER_BEEP1 DO4S
#define GAMEOVER_BEEP2 DO4S

#define TIMER_BEEP 4000

//7セグに数字を出力する用途で使用
void segment_out(int num);
//7セグに文字を出力する用途で使用
void segment_out_char(char moji);
#endif /* USER_KIOKU_GAME_PROGRAM_H_ */
