/*
 * CH32V003F4P6_Mylib.h
 *
 *  Created on: Jan 21, 2024
 *      Author: Yuki(denshi-1996)
 */

#ifndef USER_CH32V003F4P6_MYLIB_H_
#define USER_CH32V003F4P6_MYLIB_H_

//入出力関係_定義
#define MODE_OUTPUT 0b11 //50MHz スピード OUTPUT
#define MODE_OUTPUT_2MHz 0b10
#define MODE_OUTPUT_10MHz 0b01
#define MODE_INPUT 0b00
#define CNF_INPUT_ANALOG 0b00
#define CNF_INPUT_FLOAT 0b01
#define CNF_INPUT_PULL 0b10
#define CNF_OUTPUT_PUSH_PULL 0b00
#define CNF_OUTPUT_OPEN_DRAIN 0b01
#define CNF_OUTPUT_FUNC_PUSH_PULL 0b10
#define CNF_OUTPUT_FUNC_OPEN_DRAIN 0b11

//ピン_定義
#define PA1 1
#define PA2 2
#define PC0 0
#define PC1 1
#define PC2 2
#define PC3 3
#define PC4 4
#define PC5 5
#define PC6 6
#define PC7 7
#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7


#define RA3 440
#define SI3 494
#define DO4 523
#define DO4S 554
#define RE4 587
#define MI4 659
#define FA4 698
#define SO4 794
#define RA4 440 * 2
#define SI4 494 * 2
#define DO5 523 * 2
#define RE5 587 * 2
#define MI5 659 * 2
#define FA5 698 * 2
#define SO5 794 * 2

//ユーザーオプション設定
int PD7_IO_Mode(void); //PD7を標準で使用
int PD7_NRST_Mode(void); //PD7をNRST(12ms typ)で使用

//クロック設定
void Clock_init_PLL(void);
void Clock_init_8MHz(void);

//入出力設定
void pin_init(void);

//Timer2 割り込み設定
void TIM2_interrupt_init(void);

//Timer1 PWM初期化設定
void TIM1_PWM_init(void);
//TIM1_PWMを用いたtone関数 (TIM1_CH1 PD2)
void tone(int freq);
//toneを止める関数
void no_tone(void);

//FLASH書き込み関数
void FLASH_write(uint32_t Address, uint16_t Data);
//FLASH読み出し関数
uint16_t FLASH_read(uint32_t Address);
//FLASH削除関数
void FLASH_Erase(volatile uint32_t Address);


//特定ビット置き換え関数(レジスタ操作特化)
uint32_t bit_replace(uint32_t data, uint32_t byte, uint8_t len, uint8_t shift);

#endif /* USER_CH32V003F4P6_MYLIB_H_ */
