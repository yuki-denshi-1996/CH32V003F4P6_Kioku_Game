/*
 * CH32V003F4P6_Mylib.c
 *
 *  Created on: Jan 21, 2024
 *      Author: Yuki(denshi-1996)
 */

#include <ch32v00x.h>
#include "CH32V003F4P6_Mylib.h"

//  PD7を標準で使用
//  NRSTを無効にして、PD7をIOとして使用
int PD7_IO_Mode(void) {
    //フラッシュがロックされているか確認 (LOCK)
    //ロック解除シーケンス
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    FLASH->OBKEYR = 0x45670123;
    FLASH->OBKEYR = 0xCDEF89AB;
    //ロック解除シーケンスを実行したが、エラーになる場合(OBWRE)
    if (FLASH->CTLR & (1 << 9) == 0) {
        return -1;
    }

    //フラッシュの動作が終了するまで待機 (BSY)
    while(FLASH->STATR & 0x01);
    //ユーザーオプションバイトのプログラミング開始 (OBPG)
    FLASH->CTLR |= (1 << 4);
    //ユーザーオプションバイトのプログラミング開始(STRT)
    FLASH->CTLR |= (1 << 6);

    volatile uint16_t *CONFIG = (uint16_t*) 0x1FFFF802;
    *CONFIG = 0x001F;

    //フラッシュの動作が終了するまで待機 (BSY)
    while(FLASH->STATR & 0x01);

    u_int16_t check = *CONFIG;

    //ユーザーオプションバイトのプログラミング終了 (OBPG)
    FLASH->CTLR &= ~(1 << 4);
    //FLASH LOCK
    FLASH->CTLR |= (1 << 7);
    return check;
}

//  PD7をNRST(12ms typ)で使用
//  NRSTを有効にして、PD7をNRSTピンとして使用
// 戻り値　書き込んだ値
int PD7_NRST_Mode(void) {
    //フラッシュがロックされているか確認 (LOCK)
    //ロック解除シーケンス
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    FLASH->OBKEYR = 0x45670123;
    FLASH->OBKEYR = 0xCDEF89AB;
    //ロック解除シーケンスを実行したが、エラーになる場合(OBWRE)
    if (FLASH->CTLR & (1 << 9) == 0) {
        return -1;
    }

    //フラッシュの動作が終了するまで待機 (BSY)
    while(FLASH->STATR & 0x01);
    //ユーザーオプションバイトのプログラミング開始 (OBPG)
    FLASH->CTLR |= (1 << 4);
    //ユーザーオプションバイトのプログラミング開始(STRT)
    FLASH->CTLR |= (1 << 6);

    volatile uint16_t *CONFIG = (uint16_t*) 0x1FFFF802;

    *CONFIG = 0xE0;

    //フラッシュの動作が終了するまで待機 (BSY)
    while(FLASH->STATR & 0x01);

    u_int16_t check = *CONFIG;

    //ユーザーオプションバイトのプログラミング終了 (OBPG)
    FLASH->CTLR &= ~(1 << 4);
    FLASH->CTLR &= ~(1 << 5);
    //FLASH_LOCK
    FLASH->CTLR |= (1 << 7);
    return check;
}

//  クロック設定 PLL駆動
//  ADCクロック：未定
//  SYSCLK:48MHz
//  HCLK 48MHz
//  Core System Timer 48MHz
void Clock_init_PLL(void) {
    //PLL有効
    RCC->CTLR |= (1 << 24);
    //PLL安定動作まで待機
    while(RCC->CTLR & (1 << 25) == 0);

    //ADCプリスケーラ　16 (ADCPRE)
    RCC->CFGR0 = bit_replace(RCC->CFGR0, 0b11100, 5, 11);

    //クロックソース PLLに変更 (SW)
    RCC->CFGR0 = bit_replace(RCC->CFGR0, 0b10, 2, 0);

    //HCLKプリスケーラ 0 (HPRE)
    RCC->CFGR0 = bit_replace(RCC->CFGR0, 0b0000, 4, 4);
}

void Clock_init_8MHz(void) {
    //HCLKプリスケーラ ÷3 (24MHz ÷ 3 = 8MHz) (HPRE)
    RCC->CFGR0 = bit_replace(RCC->CFGR0, 0b0010, 4, 4);
}

// 入出力設定
// 注：ここではオルタネートファンクションの設定はしないこと
void pin_init(void) {
    //RAポート有効
    RCC->APB2PCENR |= (1 << 2);
    //RCポート有効
    RCC->APB2PCENR |= (1 << 4);
    //RDポート有効
    RCC->APB2PCENR |= (1 << 5);

    //PA1有効
    AFIO->PCFR1 &= ~(1 << 15);

    //PA1 プッシュプル出力
    GPIOA->CFGLR = bit_replace(GPIOA->CFGLR, MODE_OUTPUT, 2, PA1 * 4);
    GPIOA->CFGLR = bit_replace(GPIOA->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PA1 * 4 + 2);
    //PA2 プッシュプル出力
    GPIOA->CFGLR = bit_replace(GPIOA->CFGLR, MODE_OUTPUT, 2, PA2 * 4);
    GPIOA->CFGLR = bit_replace(GPIOA->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PA2 * 4 + 2);

    //PC0 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC0 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC0 * 4 + 2);
    //PC1 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC1 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC1 * 4 + 2);
    //PC2 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC2 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC2 * 4 + 2);
    //PC3 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC3 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC3 * 4 + 2);
    //PC4 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC4 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC4 * 4 + 2);
    //PC5 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC5 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC5 * 4 + 2);
    //PC6 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC6 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC6 * 4 + 2);
    //PC7 プッシュプル出力
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, MODE_OUTPUT, 2, PC7 * 4);
    GPIOC->CFGLR = bit_replace(GPIOC->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PC7 * 4 + 2);

    //PD0 プッシュプル出力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_OUTPUT, 2, PD0 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_OUTPUT_PUSH_PULL, 2,
    PD0 * 4 + 2);
    //PD2 オルタネートファンクション _ プッシュプル出力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_OUTPUT, 2, PD2 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_OUTPUT_FUNC_PUSH_PULL, 2,
    PD2 * 4 + 2);
    //PD3 アナログ入力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_INPUT, 2, PD3 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_INPUT_ANALOG, 2, PD3 * 4 + 2);
    //PD4 プルアップ入力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_INPUT, 2, PD4 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_INPUT_PULL, 2, PD4 * 4 + 2);
    GPIOD->OUTDR |= (1 << 4); //プルアップ有効
    //PD5 プルアップ入力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_INPUT, 2, PD5 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_INPUT_PULL, 2, PD5 * 4 + 2);
    GPIOD->OUTDR |= (1 << 5); //プルアップ有効
    //PD6 プルアップ入力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_INPUT, 2, PD6 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_INPUT_PULL, 2, PD6 * 4 + 2);
    GPIOD->OUTDR |= (1 << 6); //プルアップ有効
    //PD7 プルアップ入力
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, MODE_INPUT, 2, PD7 * 4);
    GPIOD->CFGLR = bit_replace(GPIOD->CFGLR, CNF_INPUT_PULL, 2, PD7 * 4 + 2);
    GPIOD->OUTDR |= (1 << 7); //プルアップ有効
}

//Timer2　割り込み設定　初期化
void TIM2_interrupt_init(void) {
    //TIM2有効 (TIM2EN)
    RCC->APB1PCENR |= (1 << 0);

    //TIM2 イベント設定 (UG)
    TIM2->SWEVGR |= (1 << 0);

    //オートリロードプレロード有効(ARPE)
    TIM2->CTLR1 |= (1 << 7);
    //TIM2 Update割り込み有効(UIE)
    TIM2->DMAINTENR |= (1 << 0);

    //TIM2プリスケーラ
    //TIM2->PSC = 47; //(PLL使用時 @48MHz)
    //TIM2->PSC = 23; //(初期状態 @24MHz)
    TIM2->PSC = 7; //@8MHz

    //TIM2 オートリロードレジスタ
    //(1kHz周期　1ms)
    //TIM2->ATRLR = 1000;
    TIM2->ATRLR = 1000;

    //TIM2 NVIC 割り込み有効
    NVIC_EnableIRQ(TIM2_IRQn);

    //TIM2カウント有効(CEN)
    TIM2->CTLR1 |= (1 << 0);
}

//Timer1 PWM_初期化設定
void TIM1_PWM_init(void) {
    //TIM1　有効
    RCC->APB2PCENR |= RCC_TIM1EN;

    //Update Event Generation有効
    TIM1->SWEVGR |= TIM_UG;

    //TIM1 CHメイン出力有効
    TIM1->BDTR |= TIM_MOE;

    //TIM1_CH1 (PD2) 出力許可
    //TIM1->CCER |= TIM_CC1E;

    //TIM1_CH1 PWMモード1
    TIM1->CHCTLR1 = bit_replace(TIM1->CHCTLR1, 0b110, 3, 4);
    //TIM1_CH1 プリロード有効
    TIM1->CHCTLR1 |= TIM_OC1PE;

    //オートリロードプリロード有効
    TIM1->CTLR1 |= TIM_ARPE;

    //1MHz (@8MHz)
    TIM1->PSC = 7;

    //1kHz動作
    TIM1->ATRLR = 1000;

    //TIM1->CH1CVR = 500;

    //TIM1 カウント有効
    TIM1->CTLR1 |= TIM_CEN;
}

//TIM1_PWMを用いたtone関数 (TIM1_CH1 PD2)
//freq:周波数
void tone(int freq) {
    //freqに入力された周波数にセット
    TIM1->ATRLR = 1000000 / freq;
    //Duty比を50%にセット
    TIM1->CH1CVR = 1000000 / freq / 2;

    //TIM1_CH1 (PD2) 出力許可
    TIM1->CCER |= TIM_CC1E;
}

//toneを止める関数
void no_tone(void) {
    //TIM1_CH1 (PD2) 出力禁止
    TIM1->CCER &= ~TIM_CC1E;
}

//FLASH書き込み関数
//Address:アドレス   | Data:書き込むデータ
void FLASH_write(volatile uint32_t Address, volatile uint16_t Data) {
    //FLASHがLOCK状態だったら解除
    if (FLASH->CTLR & FLASH_CTLR_LOCK) {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }

    //スタンダードプログラミング有効
    FLASH->CTLR |= FLASH_CTLR_PG;

    *(volatile uint16_t *)Address = Data;

    //FLASHがBUSYでなくなるまで待機
    while(FLASH->STATR & FLASH_STATR_BSY);
    FLASH->STATR |= FLASH_FLAG_EOP;

    FLASH->CTLR &= ~FLASH_CTLR_PG;

    FLASH->CTLR |= FLASH_CTLR_LOCK;
}

//FLASH読み出し関数
uint16_t FLASH_read(volatile uint32_t Address){
    volatile uint16_t *Data = (uint16_t*)Address;
    return *Data;
}

void FLASH_Erase(volatile uint32_t Address){
    //FLASHがLOCK状態だったら解除
        if (FLASH->CTLR & FLASH_CTLR_LOCK) {
            FLASH->KEYR = 0x45670123;
            FLASH->KEYR = 0xCDEF89AB;
        }

        //標準削除モード
        FLASH->CTLR |= FLASH_CTLR_PER;
        //削除する先頭ページアドレス指定
        FLASH->ADDR = Address;
        //削除開始
        FLASH->CTLR |= FLASH_CTLR_STRT;

        //削除が完了するまで待機
        while(FLASH->STATR & FLASH_STATR_BSY);

        FLASH->STATR |= FLASH_FLAG_EOP;

        //終了
        FLASH->CTLR &= ~FLASH_CTLR_PER;

}

//  ビット置き換え関数
//  引数 data:置き換え前のビット列 | byte:置き換えるビット | len:置き換えるビットの長さ | shift:シフト数
//
uint32_t bit_replace(uint32_t data, uint32_t byte, uint8_t len, uint8_t shift) {
    uint32_t mask = ~(((1 << len) - 1) << shift);
    data &= mask;
    data |= byte << shift;
    return data;
}
