/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on November 7, 2018, 5:11 PM
 */

#define _XTAL_FREQ 32768

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

int brightness=10;//A number from 1 to 12
int prevup=1;
int prevdn=1;

int pwmincr=0;

void interrupt timer_isr(void){
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        if(prevup==0 && GP0==0){ 
            if(brightness<12){
                brightness++;
            }
        }
        if(prevdn==0 && GP1==0){
            if(brightness>1){
                brightness--;
            }
        }
        prevup=GP0;
        prevdn=GP1;
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        pwmincr++;
        if(pwmincr==12){
            pwmincr=0;
            GP2=1;
        }
        if(pwmincr>=brightness){
            GP2=0;
        }
    }
}

void main(void) {
    CMCON=0b00000111;//Turn comp off
    
    //GP2 is the output sig
    TRISIO=0b000011;
    GPIO=0b000011;
        
    //Timer1 setup
    T1CON = 0b00100001;//timer1, turn it on
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    TMR1=0;
    
    //timer0 setup
    OPTION_REG=0x00;//PS=2
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    
    GP2=1;
    while(1);
    
}
