/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on March 27, 2020, 12:05 AM
 */
#define trig RA0
#define up RA4
#define dn RA5


// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

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
    ANSELA=0x00;
    
    TRISA=0b110000;
    PORTA=0b110000;
    
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

