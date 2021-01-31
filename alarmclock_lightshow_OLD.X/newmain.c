/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on November 29, 2018, 8:12 PM
 */

#define _XTAL_FREQ 12000000

// CONFIG
#pragma config FOSC = EXTRCCLK  // Oscillator Selection bits (RC oscillator: CLKOUT function on GP4/OSC2/CLKOUT pin, RC on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#include <xc.h>

int nSecIncr=128;
int nSecMax=3600;
int brtincr=1;
int brtmax=10;

int prevAlm=0;
int prevButton=1;

int shutoffFlag=0;
int incrFlag=0;

int almOn=0;
int brtncurr=0;
int pwmc=0;
int nSec=0;
int t1incr=0;


void interrupt timer1_isr(void){
    if(PIR1bits.TMR1IF==1){
       PIR1bits.TMR1IF = 0;
       
       //Timer counter
       t1incr++;
       if(t1incr==6){
           t1incr=0;
           nSec++;
       }
       
       if(nSec>=nSecIncr){
           nSec=0;
           incrFlag=1;
       }
       
       //Debounce of the alarm signal
       if(GP1==1 && prevAlm==1){
           almOn=1;
       }
       prevAlm=GP1;
       
       //Debounce of the shutoff button
       if(prevButton==1 && GP0==1){
           shutoffFlag=1;
       }
       prevButton=GP0;
    }
    
    //PWM
    if(INTCONbits.TMR0IF==1){
        if(almOn==0){
            return;
        }
        INTCONbits.TMR0IF=0;
            pwmc++;
            if(pwmc>=brtncurr){
                GP2=0;
            }
            if(pwmc==brtmax){
                pwmc=0;
                GP2=1;
            }
            
        }
    }


void main(void) {
    CMCON=0b00000111;
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
    OPTION_REG=0b00000010;//PS=8, 1.2khz
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    //There is really no way to turn on off timer0
    
    TMR1=0;
    
    while(1){
        if(shutoffFlag==1){
            shutoffFlag=0;
            brtncurr=0;
            almOn=0;
        }
        if(incrFlag==1 && almOn==1){
            incrFlag=0;
            if(brtncurr<12){
               brtncurr=brtncurr+brtincr;
           }
        }
    }
}
