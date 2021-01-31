/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on January 25, 2020, 12:19 AM
 */

#define rly GP1
#define indic GP0
#define almin GP2

// CONFIG
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

int prevstate=0;
int pwrOn=0;

void interrupt timer_ISR(void)
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        if(almin==1 && prevstate==1){
            pwrOn=1;
        }
        if(almin==0 && prevstate==0){
            pwrOn=0;
        }
        prevstate=almin;
    }
}    
void main(void) {
    //GP1 relay out, GP2 signal in, GP0 LED
    
    OSCCON=0b1100101;//Internal oscillator, 4MHz
    CMCON0=0x07; 
    ANSEL=0x00;
    
    TRISIO=0b000100;
    GPIO=0b000100;
    
    //Timer1 setup
    TMR1=0;
    T1CON = 0b00100001;//PS=4, 3.81hz
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    while(1){
        rly=pwrOn;
        indic=pwrOn;
    }
}
