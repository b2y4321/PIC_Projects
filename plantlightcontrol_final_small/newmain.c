/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on May 10, 2018, 4:39 PM
 */


/*
 * File:   newmain.c
 * Author: xyz
 *
 * Created on February 3, 2018, 8:16 AM
 */

#define _XTAL_FREQ 12000000

#define buppin RB0
#define bdnpin RB1
#define ruppin RB3
#define rdnpin RB2
#define rout RB5
#define bout RB6

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>

//Brightness vals - starting vals
int blueval=2;
int redval=2;

//Maximum brighntness setting
int maxBright=20;

//Cycle status
int bluestat=0;
int redstat=0;

int prev_bup=1;
int prev_bdn=1;
int prev_rup=1;
int prev_rdn=1;

//It works mama!!!!!!!!!!!!!!!!!!!!!!!!!!
void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF = 0;
        
        if(buppin==0 && prev_bup==0){
            if(blueval<maxBright){
                blueval++;
            }
        }
        if(bdnpin==0 && prev_bdn==0){
            if(blueval>1){
                blueval--;
            }
        }
        if(ruppin==0 && prev_rup==0){
            if(redval<maxBright){
                redval++;
            }
        }
        if(rdnpin==0 && prev_rdn==0){
            if(redval>1){
                redval--;
            }
        }
        
        prev_bup=buppin;
        prev_bdn=bdnpin;
        prev_rup=ruppin;
        prev_rdn=rdnpin;
        
        
    }
    
    
    if(INTCONbits.TMR0IF==1){
        
        INTCONbits.TMR0IF=0; 
        bluestat++;
        redstat++;
        
        //cycle starts
        if(redstat>maxBright){
            redstat=0;
            rout=1;
        }
        if(bluestat>maxBright){
            bluestat=0;
            bout=1;
        }
        
        //brightness cycle ends
        if(redstat>redval){
            rout=0;
        }
        if(bluestat>blueval){
            bout=0;
        }
    } 
}

void main(void)
{ 
    TRISB=0b00001111;
    PORTB=0b00001111;
    
    //CMCON=0b00000111;
    
    //Timer1 setup for debounce
    T1CON = 0b00100001;//timer1 off, with ps=8
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    TMR1H=0x00;
    TMR1L=0x00;
    
    
    //timer0 setup for PWM pulse
    OPTION_REG=0b00000000;//PS=2
    TMR0=0;
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    INTE=1; 
    
    //Turning everything on
    rout=1;
    bout=1;
    
    while(1);
}
 
