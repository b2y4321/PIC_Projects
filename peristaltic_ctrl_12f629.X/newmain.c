/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on March 27, 2020, 12:05 AM
 */
#define trig GP0
#define up GP4
#define dn GP5


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


int pwrmax=10;
int pwrmin=1;
int pwr=1;

int prevup=1;
int prevdn=1;

int cycnum=0;

void __interrupt timer_isr(void){
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        
        if(prevup==0 && up==0){
           if(pwr<pwrmax){
               pwr++;
           } 
        }
        
        if(prevdn==0 && dn==0){
            if(pwr>pwrmin){
                pwr--;
            }
        }
        
        prevup=up;
        prevdn=dn;
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        
        cycnum++;
        
        if(cycnum>=pwr){
            trig=0;
        }
        
        if(cycnum>=pwrmax){
            cycnum=0;
            trig=1;
        }
    }
}

void main(void) {
    OSCCON=0b01111011;//16MHz osc
    CMCON0=0x07;
    ANSEL=0x03;
    
    TRISIO=0b110000;
    GPIO=0b110000;
    GP0=1;
    
    //Timer1 setup
    T1CON = 0b00110001;//PS8, enabled (3.81hz)
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    TMR1=0;
    
     
    //timer0 setup (PWM))
    OPTION_REG=0b00000010;//PS=2 (~2000kHz)
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    
    trig=1;
    while(1);
}

