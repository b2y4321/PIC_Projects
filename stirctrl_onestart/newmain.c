/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on May 26, 2018, 11:02 PM
 */
#define _XTAL_FREQ 8000000

// CONFIG 
#pragma config FOSC = HS // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

int pwr=5;//1 to 10
int cnt1=0;
int stator=0;

void main(void) {
    TRISB=0b10111111;//RB0-RB2
    PORTB=0b10111111;
          
    OPTION_REG=0b00000010;//PS=16
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    
    //startup 1: 5v or 12v, 100dc
    RB6=1;
    __delay_ms(1000);
    stator=1;

    while(1){
        if(RB0==0){
            __delay_ms(300);
            if(RB0==0 && pwr<10){
                pwr++;
            }
        }
        
        if(RB1==0){
            __delay_ms(300);
            if(RB1==0 && pwr>1){
                pwr--;
            }
        }
    }
}

void interrupt timer0_isr(){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        cnt1++;
        if(cnt1==10){//0 to 9, in cycle
            cnt1=0;
                RB6=1;
            
        }
        if(cnt1>=pwr && stator==1){
            RB6=0;
        }
    }
}