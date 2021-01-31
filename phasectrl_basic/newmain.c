/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 8, 2018, 11:43 PM
 */
#define _XTAL_FREQ 6000000
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

//BUTTONS:
//RB4 down 
//RB5 up
//RB0 ZDC
//RB3 gate trigger

double pwr=5.0;//1 to 20 

int t1assign=0;//assignment for timer1 based on power
int t1recalc=0;//Do we need to recalculate power values?
int ZC=0;
int T1=0;

void interrupt timer_ISR(void)             // High priority interrupt
{ 
    
    if(INTF==1){
        INTF=0;
        ZC=1;
    }
    if (PIR1bits.TMR1IF == 1){ 
        PIR1bits.TMR1IF=0; 
        
        if(RB3==0){
            //fire the gates
            T1CON=0x00;
            RB3=1;
            __delay_us(650);
            RB3=0;
        }
    }  
} 

void main(void) {
    TRISB=0b00000001;
    PORTB=0b00000001;
    //Timer1 setup
    T1CON = 0x00;//off
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    //RB0 interr setup
    INTE=1;
    INTF=0;//ext interrupt flag
    INTEDG=1;//rising edge
    
    RB3=0;
    t1assign=(53036.0+(65535.0-53036.0)*(pwr/20.0));
    
    while(1){
        if(ZC==1){
            ZC=0;
            TMR1=t1assign;
            T1CON = 0b00000001;//ps1
        } 
    }
    }

