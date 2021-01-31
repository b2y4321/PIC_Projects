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

int buttonOn=0;
int count=0;
int keyg=0;
int stat=0;//1=blue up, 2=blue dn, 3=red up, 4=red dn

//Brightness vals
int blueval=2;//from 0 to 10
int redval=2;//from 0 to 10

//Cycle status
int bluestat=0;
int redstat=0;

int prev3=1;
int prev5=1;
int prev6=1;
int prev7=1;

//It works mama!!!!!!!!!!!!!!!!!!!!!!!!!!
void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF = 0;
        
        if(RB3==0 && prev3==0){
            if(blueval<10){
                blueval++;
            }
        }
        if(RB5==0 && prev5==0){
            if(blueval>1){
                blueval--;
            }
        }
        if(RB6==0 && prev6==0){
            if(redval<10){
                redval++;
            }
        }
        if(RB7==0 && prev7==0){
            if(redval>1){
                redval--;
            }
        }
        
        prev3=RB3;
        prev5=RB5;
        prev6=RB6;
        prev7=RB7;
        
        
    }
    
    
    if(INTCONbits.TMR0IF==1){
        
        INTCONbits.TMR0IF=0; 
        bluestat++;
        redstat++;
        
        //cycle starts
        if(redstat==11){
            redstat=0;
            RB0=1;
        }
        if(bluestat==11){
            bluestat=0;
            RB1=1;
        }
        
        //brightness cycle ends
        if(redstat>redval){
            RB0=0;
        }
        if(bluestat>blueval){
            RB1=0;
        }
    } 
}

void main(void)
{ 
    TRISB=0b11111000;
    PORTB=0b11111000;
    
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
    
    
    
            
    RB0=1;//blue
    RB1=1;//red
    
    while(1){
        
    }
}
 
