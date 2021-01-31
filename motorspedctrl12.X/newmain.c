/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 19, 2019, 10:01 PM
 */
#define up GP5
#define dn GP4

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

int pwr=5;
int pwrmax=10;

int stgnum=0;
int prevup=1;
int prevdn=1;

void main(void) {
    OSCCON=0b1100101;//Internal oscillator, 4MHz
    CMCON0=0x07;
    ANSEL=0x00;
    
    TRISIO=0b110000;
    GPIO=0b110000;
     
    //Timer1 setup
    T1CON = 0b00100001;//PS4 on
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

void interrupt timer_isr(){
    if(PIR1bits.TMR1IF==1){
       PIR1bits.TMR1IF = 0;
       
       if(up==0 && prevup==0){
           if(pwr<pwrmax){
               pwr++;
           }
       }
       
       if(dn==0 && prevdn==0){
           if(pwr>1){
               pwr--;
           }
       }
       
       prevup=up;
       prevdn=dn;
    }
    
    if(INTCONbits.TMR0IF==1){
       INTCONbits.TMR0IF=0;
       stgnum++;
       
       if(stgnum>=pwr){
           GP2=0;
       }
       if(stgnum>=pwrmax){
           stgnum=0;
           GP2=1;
       }
    }
    
}