/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on February 11, 2018, 2:29 PM
 */

#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

unsigned int seg[]={0X3F, //Hex value to display the number 0
                    0X06, //Hex value to display the number 1
                    0X5B, //Hex value to display the number 2
                    0X4F, //Hex value to display the number 3
                    0X66, //Hex value to display the number 4
                    0X6D, //Hex value to display the number 5
                    0X7C, //Hex value to display the number 6
                    0X07, //Hex value to display the number 7
                    0X7F, //Hex value to display the number 8
                    0X6F  //Hex value to display the number 9
                   }; //End of Array for displaying numbers from 0 to 9

int timecounter=0;
int sw=0;

int hr=1;
int min=15;
 
int hra;
int hrb;
int mina;
int minb;

void interrupt ISR(void)  {           // High priority interrupt
    if (PIR1bits.TMR1IF == 1)
    {
        PIR1bits.TMR1IF=0;
        timecounter++;
        //set to 15536
        TMR1H=0b00111100;
        TMR1L=0b10110000;
        
        if(timecounter==1200){
            sw++;
            timecounter=0;
        }
    }
    } 


void main(void) {
    TRISD=0;
    TRISC=0;
    TRISB=0;
    PORTC=0x00;
    PORTD=0x00;
    PORTB=0x00;
    
    T1CON = 0b00000001;               //Configure Timer1 interrupt
    PIE1bits.TMR1IE = 1;           
    INTCONbits.PEIE = 1;           // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    //set to 15536
    TMR1H=0b00111100;
    TMR1L=0b10110000;
    
    RB1=1;
    
    hra=hr/10;
    hrb=hr-10*hra;
    mina=min/10;
    minb=min-10*mina;
    
    while(1){
        if(sw==1){
            sw=0;
            if(min<59){
               min++; 
            }
            if(min==59 && hr<23){
                hr++;
                min=0;
            }
            if(min==59 && hr==23){
                min=0;
                hr=0;
            }
            hra=hr/10;
            hrb=hr-10*hra;
            mina=min/10;
            minb=min-10*mina;
        }
        
        //hour, tens
        
        RC0=1;
        PORTD=seg[hra];
        __delay_ms(1);
        PORTD=0x00;
        RC0=0;
        
        
        //hour, ones
        
        RC1=1;
        PORTD=seg[hrb];
        __delay_ms(1);
        PORTD=0x00;
        RC1=0;
        
        
        //min, tens
        
        RC2=1;
        PORTD=seg[mina];
        __delay_ms(1);
        PORTD=0x00;
        RC2=0;
        
        
        //min, ones
        
        RC3=1;
        PORTD=seg[minb];
        __delay_ms(1);
        PORTD=0x00;
        RC3=0;
        
        
    }
    
}


    
         
    

