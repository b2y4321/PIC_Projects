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

int count=0;
//It works mama!!!!!!!!!!!!!!!!!!!!!!!!!!
void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        TMR1H=0x00;
        TMR1L=0x00; 
        RB2=~RB2; 
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        count++;
        if(count==5){
            RB0=~RB0; 
            count=0;
        }
    } 
}    

void main(void)
{ 
    TRISB=0x00; 
     
    //Timer1 setup
    T1CON = 0b00100001;//timer1, turn it on
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    TMR1=0;
    
     
    //timer0 setup
    OPTION_REG=0b00000111;//PS=256
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    //There is really no way to turn on off timer0
    
    RB2=1;//timer1 ctrl
    RB0=1;//timer0 ctrl
    
    while(1); 
}
 
