/*
 * File:   newmain.c
 * Author: xyz
 *
 * Created on February 3, 2018, 8:16 AM
 */

#define _XTAL_FREQ 8000000


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

//Alarm durations
int alm1d=600;
int alm2d=600;
int alm3d=600;
int alm4d=600;

//Separation distance, cascade
int alm2r=600;
int alm3r=600;
int alm4r=600;

int alm1on=0;
int alm2on=0;
int alm3on=0;
int alm4on=0;


int tickcount=0;
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

void pressButton(int sel){
    __delay_ms(150);
    if(RA0==1 && sel==1){
        if(alm1on==0){
            alm1on=1;
            RB4=1;
        }
        if(alm1on==1){
            alm1on=0;
            RB4=0;
        }
    }
    if(RA1==1 && sel==2){
        if(alm2on==0){
            alm2on=1;
            RB5=1;
        }
        if(alm2on==1){
            alm2on=0;
            RB5=0;
        }
    }
    if(RA2==1 && sel==3){
        if(alm3on==0){
            alm3on=1;
            RB6=1;
        }
        if(alm3on==1){
            alm3on=0;
            RB6=0;
        }
    }
    if(RA3==1 && sel==4){
        if(alm4on==0){
            alm4on=1;
            RB7=1;
        }
        if(alm4on==1){
            alm4on=0;
            RB7=0;
        }
    }
} 

void triggerAlarm(){
    if(alm1on==1){
        
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
        //Pressing the buttons
        if(RA0==1){
            pressButton(1);
        }
        if(RA1==1){
            pressButton(2);
        }
        if(RA2==1){
            pressButton(3);
        }
        if(RA3==1){
            pressButton(4);
        }
        
        if(RA4==1){
            __delay_ms(300);
            if(RA4==1){
                triggerAlarm();
            }
        }
    }
}
 

