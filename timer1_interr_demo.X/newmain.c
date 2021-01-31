/*
 * File:   newmain.c
 * Author: xyz
 *
 * Created on February 3, 2018, 8:16 AM
 */

#define _XTAL_FREQ 4000000


// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>

int countmax=80;
int tick_count=0;
 
void interrupt ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        tick_count++;
        TMR1H=0x00;
        TMR1L=0x00; 
        if(tick_count>countmax){
            RA2=0;
            RB1=0;
        }
    }
    
    
}

void main(void)
{ 
    TRISA=0x00;
    TRISB=0x00;
    T1CON = 0b00000001;               //Configure Timer1 interrupt
    PIE1bits.TMR1IE = 1;           
    INTCONbits.PEIE = 1;           // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    TMR1H=0x00;
    TMR1L=0x00;
    
    RA2=1;
    RB1=1;
    while(1);
}
 

