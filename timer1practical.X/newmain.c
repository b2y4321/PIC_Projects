/*
 * File:   newmain.c
 * Author: xyz
 *
 * Created on February 3, 2018, 8:16 AM
 */

#define _XTAL_FREQ 4000000


// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

  
int tickcount=0;
int selector=0;
int timeval=900;
int countmax=80;//Set this to frequency and time value

void interrupt ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1) 
    {
        PIR1bits.TMR1IF=0;
        tickcount++;
        TMR1H=0x00;
        TMR1L=0x00; 
        if(tickcount>countmax){
            RA2=0;
        }
    }
      
     
} 

void pressButton(){
    if(RB1==1){
        __delay_ms(50);
        if(RB1==1){
            //Setting the selector
            if(selector<3){
                selector++;
            }
            if(selector==3){
                selector=0;
            }
            
            //Now, for doing the stuff
            timeval=900*(1+selector);//Add a new time value!
            countmax=80;//BE SURE TO CHANGE THIS VALUE
            tickcount=0;//Reset the clock!
        }
    } 
} 

void main(void)
{ 
    //Setting up the timer
    TRISA=0x00;
    TRISB=0b00000010;
    T1CON = 0b00000001;               //Configure Timer1 interrupt
    PIE1bits.TMR1IE = 1;           
    INTCONbits.PEIE = 1;           // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    TMR1H=0x00;
    TMR1L=0x00;
    
    //Now for the big stuff
    RA2=1;
    while (1){
        if(RB1==1){
            pressButton();
        }
    }
}
 

