/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on February 10, 2018, 10:51 PM
 */
#define _XTAL_FREQ 8000000

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>


void main(void) {
    TRISB=0x00;
    PORTB=0x00;
    while(1){ 
        RB0=1;
        RB1=1;
        __delay_ms(1000);
        RB0=0;
        RB1=0; 
        
        __delay_ms(1000);
    }
}
