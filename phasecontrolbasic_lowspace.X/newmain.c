/*
 * File:   newmain.c
 * Author: Briann
 *
 * Created on December 29, 2019, 7:55 PM
 */
#define _XTAL_FREQ 8000000

#define up GP4
#define dn GP5
#define gate GP2
#define pwrind GP1
//GP0 is the interrupt


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
int pwrmin=1;

int t0cnt=0;
int t0max=9;//set based on xtal frequency and timer0 prescale (8MHz, PS256))
int prevup=1;
int prevdn=1;

int gateFireTime=32;//can be adjusted, based on general practice (8MHz)
int gateFire=0;
int recalcFlag=0;
unsigned int t1val=0;

unsigned int PwrTimerVals[10]={50536,52203,53869,55536,57203,58869,60536,62203,63869,65500};

void __interrupt () timer_isr (void){
    if(INTF==1){
        INTF=0;
        T1CON=0x01;//PS1, on - 41667 pulses/cyc
        TMR1=t1val;
        pwrind=0;
    }
    
    //button debounce
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        t0cnt++;
        
        if(t0cnt>=t0max){
            t0cnt=0;
            
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
    }
    
    //Timer1 is for phase control execution
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF=0;
        if(gateFire==0){
            gateFire=1;
            TMR1=65535-gateFireTime;
            pwrind=1;
            gate=1;
        }
        if(gateFire==1){
            gateFire=0;
            T1CON=0x00;
            gate=0;
        }
    }
    
}

void main(void) {
    OSCCON=0b01110101;
    CMCON0=0x07;
    ANSEL=0x03;
    
    TRISIO=0b110001;
    GPIO=0b110001;
    
    //Timer0 setup
    OPTION_REG=0b00000111;//PS of 256, timer0 is always on
    INTCONbits.TMR0IE=1;//timer0
    INTCONbits.PEIE=1;//tmr0,1
    INTCONbits.GIE=1;//tmr0,1
    INTCONbits.TMR0IF=0;//timer0 interrupt 
    
    
    //Timer1 setup
    T1CON=0x00;//ps of 1, off
    PIE1bits.TMR1IE=1;//tmr1 only
    PIR1bits.TMR1IF=0;//Timer1 interrupt flag
    
    //Normal interrupt external
    INTE=1;
    INTF=0;
    
    
    
    gate=0;
    
    while(1){
        if(recalcFlag==1){
            recalcFlag=0;
            t1val=PwrTimerVals[pwr-1];
        }
    }
}

