/*
 * Kioku_game_program.c
 *
 *  Created on: Feb 1, 2024
 *      Author: Yuki (denshi-1996)
 */

#include <ch32v00x.h>
#include "CH32V003F4P6_Mylib.h"
#include "Kioku_game_program.h"

//7セグに数字を出力する用途で使用
//num:数字(0~9　それ以外は無効)
void segment_out(int num) {
    switch (num) {
    case 0:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x3F, 7, 0);
        break;
    case 1:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x06, 7, 0);
        break;
    case 2:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x5B, 7, 0);
        break;
    case 3:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x4F, 7, 0);
        break;
    case 4:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x66, 7, 0);
        break;
    case 5:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x6D, 7, 0);
        break;
    case 6:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x7D, 7, 0);
        break;
    case 7:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x27, 7, 0);
        break;
    case 8:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x7F, 7, 0);
        break;
    case 9:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x6F, 7, 0);
        break;
    default:
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x00, 7, 0);
        break;
    }
}

//7セグに文字を出力する用途で使用
void segment_out_char(char moji) {
    switch (moji) {
    case 'b':
    case 'B':
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x7C, 7, 0);
        break;
    case 'c':
    case 'C':
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x39, 7, 0);
        break;
    case 'e':
    case 'E':
        GPIOC->OUTDR = bit_replace(GPIOC->OUTDR, 0x79, 7, 0);
        break;
    }
}
