/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on December 2, 2018, 12:09 AM
 */

#define _XTAL_FREQ 12000000
#define acpin RB0
#define shutdn RB1
#define almpin RB2

// CONFIG
#pragma config FOSC = EXTRCCLK  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

int brtn=0;
int brtnmax=12;
int switchsec=5;
int shdnsec=3600;

//status
int almOn=0;
int pwmstat=0;
int counter=0;
int sec=0;

//button_debchar
int prevalm=0;
int prevbutton=0;


void interrupt timer_isr(void){
    //zdc timer
    if(INTF==1){
        INTF=0;
        if(almOn==1){
            counter++;
            if(counter==60){
                counter=0;
                sec++;
            } 
            if(sec==switchsec && brtn<brtnmax){
                brtn++;
                sec=0;
            }
        }
    }
    
    //button and alm debounce
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        if(prevbutton==0 && shutdn==0){
            almOn=0;
        }
        if(prevalm==1 && almpin==1){
            almOn=1;
        }
        prevbutton=shutdn;
        prevalm=almpin;
    }
    
    //PWM
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        
        if(almOn==1){
        pwmstat++;
        if(pwmstat>=brtn){
            RB3=0;
        }
        if(pwmstat==brtnmax){
            RB3=1;
            pwmstat=0;
        }
        }
    }
}

void main(void) {
    TRISB=0b00000111;
    PORTB=0b00000011;
    
    T1CON = 0b00110001;//PS=8
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    TMR1=0;

    OPTION_REG=0b00000010;//PS=8
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    
    INTE=1;//external interr
    INTF=0;
    
    while(1){
        if(sec>=shdnsec){
            almOn=0;
            sec=0;
        }
        if(almOn==0){
            RB3=0;
            sec=0;
            brtn=0;
        }
    }

}
