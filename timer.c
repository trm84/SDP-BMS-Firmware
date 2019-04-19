/*
 * File:   timer.c
 * Author: trm84
 *
 * Created on February 7, 2019, 11:36 AM
 */

#include "timer.h"

void timerSetup(){
    timer0Setup();
    timer2Setup();
}

void timer2Setup(){
    CCP2CON = 0x06; //Generate Software Interrupt only
    PIE1bits.TMR2IE = 1; //Timer 2 Interrupt Enable
    PR2 = 254; //255 - 2 gives 5mS timer
    T2CON = 0x4F; // Postscaler = 1:16, Prescaler = 64, Timer2 is on
}

void timer0Setup(){
    OPTION_REG = 0b10001110; //1110 = 1:128
    INTCONbits.TMR0IE = 1; //TIMER0 INTERRUPT EN
}