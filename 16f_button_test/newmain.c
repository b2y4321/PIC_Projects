/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on May 10, 2018, 4:39 PM
 */


/*
 * File:   newmain.c
 * Author: xyz
 *
 * Created on February 3, 2018, 8:16 AM
 */

#define _XTAL_FREQ 10000000

//pin definitions (to test shit)
#define pina RA0
#define pinb RA1
#define outa RB2
#define outb RB3

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

int prev1=1;
int prev2=1;

//It works mama!!!!!!!!!!!!!!!!!!!!!!!!!!
void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF = 0;
        if(pina==0 && prev1==0){
            outa=~outa;
        }
        if(pinb==0 && prev2==0){
            outb=~outb;
        } 
        prev1=pina;
        prev2=pinb;
    }
}

void main(void)
{ 
    TRISA=0b00000011;
    PORTA=0b00000011;
    TRISB=0x00;
    PORTB=0x00;
    CMCON=0x07;//this is necc if your use RA0-RA2 ports
    
    //Timer1 setup for debounce
    T1CON = 0b00100001;
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    TMR1=0;
    
    outa=1;
    outb=1;
    
    while(1){
        
    }
}
 
