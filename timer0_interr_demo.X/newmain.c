/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 3, 2018, 11:48 PM
 */
#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR pin function select (RA5/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOD Reset enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Code Protection bits (Program memory code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
//This code Works!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int count=0;

void interrupt timer0_isr(void){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        count++; 
        if(count==10){
            RB1=~RB1;
            count=0;
        }
    }  
}   
 
void main(void) {
    TRISB=0x00;
    
    
    OPTION_REG=0b00000111;//PS=256
    TMR0=0;
    INTCONbits.TMR0IE = 1; 
    INTCONbits.PEIE = 1;            
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IF=0; 
    //There is really no way to turn on off timer0
    
    
    RB1=1;
    
    while(1){
        
    }
}
