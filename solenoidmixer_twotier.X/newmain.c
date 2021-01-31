/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on March 22, 2019, 9:11 PM
 */

#define _XTAL_FREQ 10000000

// CONFIG
#pragma config FOSC = XT  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)

#include <xc.h>
//number of half cycles per second
double freqmin=1.0;
double freqmax=14.0;
double freq=1.0;

//other vars
int prevup=1;
int prevdn=1;
int recalcFlag=1;//Do we need to recalc TMR1 val
//int numSol=1;
int t1val=3036;
int fireCount=0;//How many times has the timer overflowed?
int debounceCount=0;//Debounce counter for buttons
int fireFlag=0;

void interrupt timer_isr(){
    //timer1 regime 
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        TMR1=t1val;
        //TMR1=3036;
        fireCount++;
        
        if(fireCount==5){
            fireCount=0;
            fireFlag=1;
        }
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        debounceCount++;
        if(debounceCount==5){
            debounceCount=0;
            
            if(prevup==0 && RA0==0){
                if(freq<14.0){
                    freq=freq+1.0;
                    recalcFlag=1;
                }
            }
            if(prevdn==0 && RA1==0){
                if(freq>1.0){
                    freq=freq-1.0;
                    recalcFlag=1;
                }
            }
            
            prevup=RA0;
            prevdn=RA1;
        }
    }
}

void main(void) {
    TRISA=0b00000011;
    TRISB=0x00;
    PORTA=0b00000011;
    PORTB=0x00;
    
    CMCON=0x07;//for RA0 and RA1
    //Timer1 setup
    T1CON = 0b00110001;//PS of 8
    PIE1bits.TMR1IE = 1;    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    TMR1=t1val;
    //TMR1=3036;
    
    //timer0 setup
    OPTION_REG=0b00000111;//PS=256
    TMR0=0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF=0;
    
    RB0=1;
    RB1=0; 
    
    while(1){ 
        if(recalcFlag==1){
            recalcFlag=0;
            t1val=(int)(65536.0-62500.0*(1.0/freq));//recalc timer1 val
        }
        if(fireFlag==1){
            fireFlag=0;
            RB0=~RB0;
            RB1=~RB1;
            //__delay_ms(100);
            
        }
    }
}

