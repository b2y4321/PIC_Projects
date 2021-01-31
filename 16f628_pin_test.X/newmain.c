/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 22, 2020, 3:15 PM
 */
#define _XTAL_FREQ 16000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
    TRISA=0x00;
    PORTA=0x00;
    TRISB=0x00;
    PORTB=0x00;
    
    while(1){
        RA0=1;
        
        RB0=1;
        RB1=1;
        RB2=1;
        RB3=1;
        RB5=1;
        RB6=1;
        RB7=1;
        
        __delay_ms(500);
        
        RA0=0;
        
        RB0=0;
        RB1=0;
        RB2=0;
        RB3=0;
        RB5=0;
        RB6=0;
        RB7=0;
        
        __delay_ms(500);
    }
}
