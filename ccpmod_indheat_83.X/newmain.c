/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on October 22, 2020, 12:41 AM
 */
// CONFIG
#define _XTAL_FREQ 4000000

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

void main(void) {
    //4MHz oscillator
    
    TRISIO=0x00;
    GPIO=0x00;
    
    //oscillator setup: For 25khz CCP1 signal
    TMR2IF=0;
    PR2=66;//This is where timer2 turns over (it count from 0 to PR2)
    CCP1CON=0b00101111;//PWM mode on CCP1 pin
    CCPR1L=0b100001;//duty cycle 8 msb
    T2CON=0b00000100;//pres=1, postS=1, tmr2 on
    
    
    
    
    
    

    
    while(1){
        GP1=1;
        __delay_ms(500);
        GP1=0;
        __delay_ms(500);
    }
}

