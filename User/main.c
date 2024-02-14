/*
 *  記憶ゲーム V1.0
 *  2024/02/01
 *  Yuki (denshi-1996)
 */

#include <stdlib.h>
#include <ch32v00x.h>
#include "CH32V003F4P6_Mylib.h"
#include "Kioku_game_program.h"

#define DEBUG_MODE 0

void segment_print(int num, uint8_t wait);
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
volatile unsigned int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0;
volatile unsigned int cnt6 = 0, cnt7 = 0, cnt8 = 0, cnt9 = 0, cnt10 = 0;
volatile int mode = MODE_START;

int main(void) {

    // 赤：1　緑：2　青：3　黄:4
    // 乱数で生成された点灯パターンが入る配列
    static unsigned char led_patt[1000] = { 0 };
    // スコア(どこまでいったか)(0からスタート)
    static int led_num = 0;
    // 入力時、何箇所目か
    static int led_num_now = 0;
    // 何色を入力したか
    static int push_led = 0;
    // 難易度用変数
    static int dif_wait_time;

    // タイマー時間
    static int timer_tim = 3;

    // メインクロックを8MHzに設定
    Clock_init_8MHz();
    // IOピン設定
    pin_init();
    // TIM2割り込み設定
    TIM2_interrupt_init();
    // TIM1 PWM1_CH1 PWM出力設定
    TIM1_PWM_init();

    // 初期状態で光らないように
    GPIOC->OUTDR = 0x00;
    GPIOA->OUTDR |= (1 << LED_BLUE);
    GPIOA->OUTDR |= (1 << LED_GREEN);
    GPIOD->OUTDR |= (1 << LED_RED);

    // 起動した時から、ボタン長押しを防ぐために、ボタンが離されるまで待機
    while ((GPIOD->INDR & (1 << SW_RED)) == 0)
    ;

    // フラッシュメモリが初期値の場合、リセット
    if (FLASH_read(HISCORE_FLASH) == 0xFFFF) {
        FLASH_write(HISCORE_FLASH, 0);
    }
    if (FLASH_read(DIF_TIME_FLASH) == 0xFFFF) {
        FLASH_write(DIF_TIME_FLASH, 5);
    }

    // 難易度
    switch (FLASH_read(DIF_TIME_FLASH)) {
    case 1:
        dif_wait_time = 1000;
        break;
    case 2:
        dif_wait_time = 850;
        break;
    case 3:
        dif_wait_time = 700;
        break;
    case 4:
        dif_wait_time = 600;
        break;
    case 5:
        dif_wait_time = 450;
        break;
    case 6:
        dif_wait_time = 400;
        break;
    case 7:
        dif_wait_time = 300;
        break;
    case 8:
        dif_wait_time = 200;
        break;
    case 9:
        dif_wait_time = 100;
        break;
    default:
        dif_wait_time = 500;
        break;
    }

    while (1)
    {
        switch (mode)
        {
            case MODE_START:
            // ボタンが押されるまで待機
            if (cnt1 >= 500)
            {
                cnt1 = 0;
                GPIOC->OUTDR ^= SEG_DOT;
            }
            // 乱数をランダムにするため
            rand();

            // 黄色＋青で難易度変更
            if ((GPIOD->INDR & (1 << SW_BLUE)) == 0 && (GPIOD->INDR & (1 << SW_YELLOW)) == 0)
            {
                // メモリから削除する関係で、一時的に保持
                int hiscore_temp = FLASH_read(HISCORE_FLASH);
                int dif_temp = FLASH_read(DIF_TIME_FLASH);
                dif_temp++;
                if (dif_temp >= 10)
                {
                    dif_temp = 1;
                }
                segment_print(dif_temp, 1);
                FLASH_Erase(HISCORE_FLASH);
                FLASH_write(HISCORE_FLASH, hiscore_temp);
                FLASH_write(DIF_TIME_FLASH, dif_temp);
                while ((GPIOD->INDR & (1 << SW_BLUE)) == 0 && (GPIOD->INDR & (1 << SW_YELLOW)) == 0)
                ;

                // 難易度
                switch (FLASH_read(DIF_TIME_FLASH))
                {
                    case 1:
                    dif_wait_time = 850;
                    break;
                    case 2:
                    dif_wait_time = 750;
                    break;
                    case 3:
                    dif_wait_time = 550;
                    break;
                    case 4:
                    dif_wait_time = 450;
                    break;
                    case 5:
                    dif_wait_time = 400;
                    break;
                    case 6:
                    dif_wait_time = 350;
                    break;
                    case 7:
                    dif_wait_time = 250;
                    break;
                    case 8:
                    dif_wait_time = 200;
                    break;
                    case 9:
                    dif_wait_time = 100;
                    break;
                    default:
                    dif_wait_time = 500;
                    break;
                }
            }

            // 黄色＋赤でハイスコアリセット
            if ((GPIOD->INDR & (1 << SW_RED)) == 0 && (GPIOD->INDR & (1 << SW_YELLOW)) == 0)
            {
                GPIOC->OUTDR &= ~SEG_DOT;
                int dif_temp = FLASH_read(DIF_TIME_FLASH);
                FLASH_Erase(HISCORE_FLASH);
                FLASH_write(DIF_TIME_FLASH, dif_temp);
                FLASH_write(HISCORE_FLASH, 0);
                segment_out_char('C');
                cnt1 = 0;
                while (cnt1 < 600)
                ;
                cnt1 = 0;
                mode = MODE_GAME_START;
            }
#if DEBUG_MODE
            if ((GPIOD->INDR & (1 << SW_GREEN)) == 0 && (GPIOD->INDR & (1 << SW_YELLOW)) == 0)
            {
                for (int i = 0; i <= 20; i++)
                {
                    led_patt[i] = i % 4 + 1;
                }
                led_num = 20;
            }
#endif
            // SW_REDが押されたら
            if ((GPIOD->INDR & (1 << SW_RED)) == 0)
            {
                GPIOC->OUTDR &= ~SEG_DOT;
                // モードをGAME_STARTに変更
                mode = MODE_GAME_START;
                cnt1 = 0;// MODE_GAME_STARTでも使用するため
            }
            // SW_GREENが押されたら
            if ((GPIOD->INDR & (1 << SW_GREEN)) == 0)
            {
                GPIOC->OUTDR &= ~SEG_DOT;
                // モードをTIMER_MODEに変更
                mode = MODE_TIMER_SELECT;

                tone(RED_BEEP);
                segment_print(timer_tim, 0);
                cnt2 = 0;
                while (cnt2 < 100)
                ;// チャタリング対策
                no_tone();
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_GREEN)) == 0)
                ;
                while (cnt2 < 100)
                ;// チャタリング対策

                cnt1 = 0;// MODE_GAME_STARTでも使用するため
            }
            break;
            case MODE_GAME_START:
            // ゲーム開始時のハイスコア表示 & 音,色確認
            if (cnt1 < 700)
            {
                segment_print(FLASH_read(HISCORE_FLASH), 1);
                cnt1 = 700;
            }
            if (cnt1 >= 700 && cnt1 < 1200)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(RED_BEEP);
            }
            if (cnt1 >= 1200 && cnt1 < 1700)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(GREEN_BEEP);
            }
            if (cnt1 >= 1700 && cnt1 < 2200)
            {
                GPIOA->OUTDR &= ~(1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(BLUE_BEEP);
            }
            if (cnt1 >= 2200 && cnt1 < 2700)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(YELLOW_BEEP);
            }
            if (cnt1 >= 2700 && cnt1 < 3200)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                no_tone();
            }
            if (cnt1 >= 3200)
            {
                mode = MODE_RANDOM;
            }
            break;
            case MODE_RANDOM:
            // 乱数を生成し、変数に格納するMode
            led_patt[led_num] = rand() % 4 + 1;
            cnt2 = 0;
            cnt3 = 0;
            led_num_now = 0;
            mode = MODE_LED_BEEP;
            break;
            case MODE_LED_BEEP:
            // 光とビーブオンを出力する
            if (led_patt[led_num_now] == RED)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(RED_BEEP);
            }
            else if (led_patt[led_num_now] == GREEN)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(GREEN_BEEP);
            }
            else if (led_patt[led_num_now] == BLUE)
            {
                GPIOA->OUTDR &= ~(1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(BLUE_BEEP);
            }
            else if (led_patt[led_num_now] == YELLOW)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(YELLOW_BEEP);
            }
            if (cnt3 >= dif_wait_time)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                no_tone();
                led_num_now++;
                while (cnt3 < dif_wait_time + 100)
                ;
                cnt2 = 0;
                cnt3 = 0;

                if (led_num_now > led_num)
                {
                    led_num_now = 0;
                    mode = MODE_WAIT;
                }
            }
            //            if(cnt2 > 500){
            //                cnt2 = 0;
            //                mode = MODE_WAIT;
            //                no_tone();
            //                GPIOA->OUTDR |= (1 << LED_BLUE);
            //                GPIOA->OUTDR |= (1 << LED_GREEN);
            //                GPIOD->OUTDR |= (1 << LED_RED);
            //            }
            break;
            case MODE_WAIT:
            // ボタンを押すまでの待ち時間
            // 赤が押されたら
            if ((GPIOD->INDR & (1 << SW_RED)) == 0)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(RED_BEEP);
                push_led = RED;
                cnt2 = 0;
                while (cnt2 < 20)
                ; // チャタリング対策
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_RED)) == 0)
                ;
                mode = MODE_JUDGE;
            }
            // 緑が押されたら
            if ((GPIOD->INDR & (1 << SW_GREEN)) == 0)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(GREEN_BEEP);
                push_led = GREEN;
                cnt2 = 0;
                while (cnt2 < 20)
                ; // チャタリング対策
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_GREEN)) == 0)
                ;
                mode = MODE_JUDGE;
            }
            // 青が押されたら
            if ((GPIOD->INDR & (1 << SW_BLUE)) == 0)
            {
                GPIOA->OUTDR &= ~(1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                tone(BLUE_BEEP);
                push_led = BLUE;
                cnt2 = 0;
                while (cnt2 < 20)
                ; // チャタリング対策
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_BLUE)) == 0)
                ;
                mode = MODE_JUDGE;
            }
            // 黄が押されたら
            if ((GPIOD->INDR & (1 << SW_YELLOW)) == 0)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
                tone(YELLOW_BEEP);
                push_led = YELLOW;
                cnt2 = 0;
                while (cnt2 < 20)
                ; // チャタリング対策
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_YELLOW)) == 0)
                ;
                mode = MODE_JUDGE;
            }

            if (cnt2 < dif_wait_time * 2)
            {
                if (cnt7 > 400)
                {
                    GPIOC->OUTDR ^= SEG_DOT;
                    cnt7 = 0;
                }
            }
            if (cnt2 > dif_wait_time * 2 && cnt2 < dif_wait_time * 4)
            {
                if (cnt7 > 150)
                {
                    GPIOC->OUTDR ^= SEG_DOT;
                    cnt7 = 0;
                }
            }
            if (cnt2 > dif_wait_time * 4)
            {
                if (cnt7 > 50)
                {
                    GPIOC->OUTDR ^= SEG_DOT;
                    cnt7 = 0;
                }
            }
            if (cnt2 > dif_wait_time * 6)
            {
                GPIOC->OUTDR &= ~SEG_DOT;
                mode = MODE_GAMEOVER;
                cnt5 = 0;
            }

            break;
            case MODE_JUDGE:
            // 押された値が正しいか判断＆次に以降するか判断
            // すぐに判断せず、しばらくは光＆音を出力
            if (cnt2 >= (dif_wait_time - 20))
            {
                if (dif_wait_time < 200)
                {
                    cnt2 = 0;
                    while (cnt2 <= 100)
                    ; // 連続で認識してしまうため、待機
                }
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
                no_tone();
                if (led_patt[led_num_now] == push_led)
                {
                    // 正解
                    led_num_now++;
                    cnt2 = 0;
                    cnt3 = 0;
                    mode = MODE_WAIT;

                    if (led_num_now > led_num)
                    {
                        mode = MODE_CLEAR_ONCE;
                        led_num++;
                        cnt4 = 0;
                    }
                }
                else
                {
                    // 不正解
                    mode = MODE_GAMEOVER;
                    cnt5 = 0;
                }
            }

            break;
            case MODE_CLEAR_ONCE:
            // クリアした場合
            segment_print(led_num, 1);
            cnt4 = 0;
            mode = MODE_CLEAR;
            break;
            case MODE_CLEAR:
            if (cnt4 > 500)
            {
                mode = MODE_RANDOM;
            }
            break;
            case MODE_GAMEOVER:
            // ゲームオーバー タイムアウト＆間違えた場合
            segment_print(led_num, 0);
            cnt5 = 0;
            // ハイスコア更新
            if (FLASH_read(HISCORE_FLASH) < led_num)
            {
                int dif_temp = FLASH_read(DIF_TIME_FLASH);
                FLASH_Erase(HISCORE_FLASH);
                FLASH_write(DIF_TIME_FLASH, dif_temp);
                FLASH_write(HISCORE_FLASH, led_num);
            }
            mode = MODE_GAMEOVER_RUN;
            break;

            case MODE_GAMEOVER_RUN:
            if (led_patt[led_num_now] == RED)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
            }
            else if (led_patt[led_num_now] == GREEN)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
            }
            else if (led_patt[led_num_now] == BLUE)
            {
                GPIOA->OUTDR &= ~(1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);
            }
            else if (led_patt[led_num_now] == YELLOW)
            {
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR &= ~(1 << LED_GREEN);
                GPIOD->OUTDR &= ~(1 << LED_RED);
            }
            if (cnt5 > 100 && cnt5 < 200)
            {
                tone(GAMEOVER_BEEP1);
            }
            if (cnt5 >= 200 && cnt5 < 250)
            {
                no_tone();
            }
            if (cnt5 >= 250 && cnt5 < 500)
            {
                tone(GAMEOVER_BEEP2);
            }
            if (cnt5 > 700)
            {
                no_tone();
                GPIOA->OUTDR |= (1 << LED_BLUE);
                GPIOA->OUTDR |= (1 << LED_GREEN);
                GPIOD->OUTDR |= (1 << LED_RED);

                // mode = MODE_START;
                NVIC_SystemReset();
            }
            break;
            case MODE_TIMER_SELECT:
            if((GPIOD->INDR & (1 << SW_RED)) == 0) {
                NVIC_SystemReset();
            }

            // 青が押されたら
            if ((GPIOD->INDR & (1 << SW_BLUE)) == 0)
            {
                tone(TIMER_BEEP);
                timer_tim--;
                if (timer_tim <= 0)
                {
                    timer_tim = 9;
                }
                segment_print(timer_tim, 0);
                cnt2 = 0;
                while (cnt2 < 100)
                ; // チャタリング対策
                no_tone();
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_BLUE)) == 0)
                ;
                while (cnt2 < 100)
                ;// チャタリング対策
                cnt9 = 0;
            }

            // 黄が押されたら
            if ((GPIOD->INDR & (1 << SW_YELLOW)) == 0)
            {
                tone(TIMER_BEEP);
                timer_tim++;
                if (timer_tim >= 10)
                {
                    timer_tim = 1;
                }
                segment_print(timer_tim, 0);
                cnt2 = 0;
                while (cnt2 < 100)
                ; // チャタリング対策
                no_tone();
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_YELLOW)) == 0)
                ;
                while (cnt2 < 100)
                ;// チャタリング対策
                cnt9 = 0;
            }

            if ((GPIOD->INDR & (1 << SW_GREEN)) == 0)
            {
                mode = MODE_TIMER_SECTION;
                cnt8 = 0;
                cnt10 = 0;
                tone(TIMER_BEEP);
            }

            //segment_print(timer_tim, 0);

            if(cnt9 <= 500) {
                segment_out(timer_tim);
            }
            if(cnt9 > 500) {
                segment_out(-1);
            }
            if(cnt9 > 1000) {
                cnt9 = 0;
            }
            break;
            case MODE_TIMER_SECTION:
            if(cnt10 > 100) {
                no_tone();
            }

            if(cnt8 >= 60 * 1000) {
                timer_tim--;
                cnt8 = 0;
            }

            if (timer_tim <= 0)
            {
                mode = MODE_TIMER_END;
                cnt9 = 0;
                cnt10 = 0;
            }

            if(cnt9 <= 500) {
                GPIOC->OUTDR |= SEG_DOT;
            }
            if(cnt9 > 500) {
                GPIOC->OUTDR &= ~SEG_DOT;
            }
            if(cnt9 > 1000) {
                cnt9 = 0;
            }

            if((GPIOD->INDR & (1 << SW_RED)) == 0) {
                NVIC_SystemReset();
            }

            segment_out(timer_tim - 1);
            break;
            case MODE_TIMER_END:
            if (cnt9 < 75)
            {
                tone(TIMER_BEEP);
            }
            if (cnt9 >= 75 && cnt9 < 75 * 2)
            {
                no_tone();
            }
            if (cnt9 >= 75 * 2 && cnt9 < 75 * 3)
            {
                tone(TIMER_BEEP);
            }
            if (cnt9 >= 75 * 3 && cnt9 < 75 * 4)
            {
                no_tone();
            }
            if (cnt9 >= 75 * 4 && cnt9 < 75 * 5)
            {
                tone(TIMER_BEEP);
            }
            if (cnt9 >= 75 * 5 && cnt9 < 75 * 6)
            {
                no_tone();
            }
            if (cnt9 >= 75 * 6 && cnt9 < 75 * 7)
            {
                tone(TIMER_BEEP);
            }
            if (cnt9 >= 75 * 7 && cnt9 < 75 * 8)
            {
                no_tone();
            }
            if (cnt9 > 1000)
            {
                cnt9 = 0;
            }

            if(cnt10 < 500) {
                segment_out(0);
            }
            if(cnt10 >= 500) {
                segment_out(-1);
            }
            if(cnt10 > 1000) {
                cnt10 = 0;
            }

            if((GPIOD->INDR & (1 << SW_GREEN)) == 0) {
                no_tone();
                timer_tim = 3;
                mode = MODE_TIMER_SELECT;
                tone(RED_BEEP);
                cnt2 = 0;
                while (cnt2 < 100)
                ; // チャタリング対策
                no_tone();
                cnt2 = 0;
                while ((GPIOD->INDR & (1 << SW_YELLOW)) == 0)
                ;
                while (cnt2 < 100)
                ;// チャタリング対策
            }

            break;
        }
    }
}

// ２桁以上を表示するための関数
// num:セグメントLEDに表示する数  | wait:最後のウェイトタイムを有効＆無効選択 0:無効 それ以外：有効
void segment_print(int num, uint8_t wait) {
    cnt6 = 0;

    // 3桁以上
    if (num >= 100) {
        segment_out(num / 100);
        while (cnt6 < SEG_SEQUENCE_WAIT_TIME)
        ;
        cnt6 = 0;
        segment_out(-1);
        while (cnt6 < 100)
        ;
        cnt6 = 0;
        num %= 100;
        segment_out(num / 10);

        while (cnt6 < SEG_SEQUENCE_WAIT_TIME)
        ;

        cnt6 = 0;
        segment_out(-1);
        while (cnt6 < 100)
        ;
        cnt6 = 0;
        num %= 10;
        segment_out(num);

        if (wait)
            while (cnt6 < SEG_SEQUENCE_1DEG_WAIT_TIME)
            ;
    }

    else if (num >= 10) { // 2桁以上
        num %= 100;
        segment_out(num / 10);
        while (cnt6 < SEG_SEQUENCE_WAIT_TIME)
        ;
        cnt6 = 0;

        segment_out(-1);
        while (cnt6 < 100)
        ;
        cnt6 = 0;
        num %= 10;
        segment_out(num);
        if (wait)
            while (cnt6 < SEG_SEQUENCE_1DEG_WAIT_TIME)
            ;
    }

    else { // 1桁
        segment_out(num);
        if (wait)
            while (cnt6 < SEG_SEQUENCE_1DEG_WAIT_TIME)
            ;
    }
}

// TIM2 割り込み
// 割り込み周期　(1ms割り込み)
void TIM2_IRQHandler(void) {
    // TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    // TIM2 割り込み解除(UIF)
    TIM2->INTFR &= ~(1 << 0);

    cnt1++; // ゲームスタート前の点滅で使用
    cnt2++; // タイムアップ用
    cnt3++; // 表示の際に使用
    cnt4++; // クリア用に使用
    cnt5++; // ゲームオーバー用
    cnt6++; // 7セグメント用(segment_print)
    cnt7++; // 残り時間点滅用
    cnt8++; // タイマー用
    cnt9++; // タイマー終了時ブザー
    cnt10++;
}
