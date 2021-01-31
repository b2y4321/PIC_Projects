/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 8, 2018, 11:43 PM
 */
#define _XTAL_FREQ 8000000

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

void interrupt timer_ISR(void)             // High priority interrupt
{
    if(INTF==1){
        INTF=0;
        RB2=~RB2;
    }
    
}

void main(void) {     
    TRISB=0b00000001;
    PORTB=0b00000001;
    
    INTCONbits.GIE =1;
    INTE=1;
    INTF=0;
    INTEDG=0;
    
    RB2=1;
    
    while(1);
}
