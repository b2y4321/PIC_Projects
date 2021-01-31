/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on May 26, 2018, 12:38 AM
 */
#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

void main(void) {
    TRISB=0b11111000;
    PORTB=0b11111000;
    
    if(RB3==0){
        __delay_ms(300);
        if(RB3==0){
            RB1=1;
            __delay_ms(500);
            RB1=0;
        }
    }
    if(RB5==0){
        __delay_ms(300);
        if(RB5==0){
            RB1=1;
            __delay_ms(500);
            RB1=0;
        }
    }
    if(RB6==0){
        __delay_ms(300);
        if(RB6==0){
            RB1=1;
            __delay_ms(500);
            RB1=0;
        }
    }
    if(RB7==0){
        __delay_ms(300);
        if(RB7==0){
            RB1=1;
            __delay_ms(500);
            RB1=0;
        }
    }
}
