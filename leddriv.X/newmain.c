/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on March 29, 2018, 6:28 PM
 */
#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on GP4/OSC2/CLKOUT and GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

double pw=0.9;
void main(void) {
    TRISIO=0b111110;//Pin 0 is opt
    T1CON = 0b00000001;               //Configure Timer1 interrupt
    PIE1bits.TMR1IE = 1;           
    INTCONbits.PEIE = 1;           // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    
    TMR1H=(long)(24767+8000*(1.0-pw));
    TMR1L=(long)(32767);
    
    while(1){
        
    }
}

void interrupt timer1_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0;
        TMR1H=(long)(24767+8000*(1.0-pw));
        TMR1L=(long)(32767);
        GP0=~GP0;
    } 
    
    
} 
