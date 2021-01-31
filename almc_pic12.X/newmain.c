/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 5, 2019, 1:22 PM
 */
#define _XTAL_FREQ 10000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High speed crystal/resonator on GP4/OSC2/CLKOUT and GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int brt=1;//Starting brightness
int brtmax=10;//Maximum brightness 
int nSecMax=1200;//At what time after max brightness does alarm shut off (secs)
int nSecIncr=10;//How often to increment brightness? (secs)

//System vars
int almOn=0;//is alarm on?
int cycnum=0;//for PWM
int cnt=0;//counting of the 5hz signal, turns over every second (i.e. 5 pulses)
int nSec=0;//Number of secs elapsed since alarm on, or last brightness increment


void main(void) {
    CMCON=0x07;
    
    TRISIO=0b000110;
    GPIO=0b000110;
     
    //Timer1 setup to produce 5hz interrupt
    T1CON = 0b00100000;//timer1 off, PS8
    PIE1bits.TMR1IE = 1;     
    INTCONbits.PEIE = 1;
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0; 
    
    TMR1=3036;
    
    //timer0 setup, pwm
    OPTION_REG=0x00;//PS=256
    TMR0=0;
    INTCONbits.TMR0IE = 1;  
    INTCONbits.TMR0IF=0; 
    
    
    while(1){
        //The decision: Do we debounce or not?
        if(GP2==1){
            __delay_ms(300);
            if(GP2==1){
                almOn=1;
                T1CON = 0b00110001;//timer1 on, PS8
            }
        }
        
        //alarm off button debounce
        if(GP1==0){
            __delay_ms(300);
            if(GP1==0){
                almOn=0;
                T1CON = 0b00110000;//timer1 off, PS8
                nSec=0;
            }
        }
        
        if(nSec>=nSecIncr){
            if(brt<brtmax){
                brt++;
                nSec=0;
            }
        }
        
        if(nSec>=nSecMax){
           almOn=0;
           T1CON = 0b00110000;//timer1 off, PS8
           nSec=0;
        }
        
    }
}

void interrupt timer_isr(){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        cycnum++;
        
        if(almOn==1){
            if(cycnum>brt){
                GP0=0;
            }
            if(cycnum>brtmax){
                GP0=1;
                cycnum=0;
            }
        }
    }
    
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0; 
        
        TMR1=3036;
        cnt++;
        
        if(cnt==5){
            cnt=0;
            nSec++;
        }
    }
}
