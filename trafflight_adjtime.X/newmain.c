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


int count1=0;
int count2=0;

int maxTime=120;//40 secs delay max
int minTime=6;//1 sec delay min

//RB2 is red
//RB3 is yellow
//RB5 is green
int prevup=1;//controlled by RB0
int prevdn=1;//controlled by RB1

int changecnt0=60;//green to red, and red to green transition
int changecnt1=12;//This is fixed! Y to R transition

int cycNum=0;
int upFlag=0;
int dnFlag=0;

int cycchangeFlag=0;


void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        TMR1=3036;
        
        count1++;
        count2++;
        
        //Up and dn buttons debouncer
        if(count1==2){
            count1=0;
            if(prevup==0 && RB0==0){
                upFlag=1;
            }
            if(prevdn==0 && RB1==0){
                dnFlag=1;
            }
            prevup=RB0;
            prevdn=RB1;
        }
        
        //Cycle change
        if(cycNum==0 && count2>=changecnt0){
            count2=0;
            cycchangeFlag=1;
        }
        if(cycNum==1 && count2>=changecnt1){
            count2=0;
            cycchangeFlag=1;
        }
        if(cycNum==2 && count2>=changecnt0){
            count2=0;
            cycchangeFlag=1;
        }
        
    }
    
    
}    

void main(void)
{ 
    TRISB=0b00000011; 
    
    //Timer1 setup
    T1CON = 0b00100001;
    PIE1bits.TMR1IE = 1;      
    INTCONbits.PEIE = 1;
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    TMR1=0;
    
    //Green light
    RB2=1;
    RB3=0;
    RB5=0;
    
    while(1){
        //Changing lights
        if(cycchangeFlag==1){
            cycchangeFlag=0;
            
            if(cycNum==0){
                RB2=0;
                RB3=1;
            }
            if(cycNum==1){
                RB3=0;
                RB5=1;
            }
            if(cycNum==2){
                RB5=0;
                RB2=1;
            }
            cycNum++;
            if(cycNum==3){
                cycNum=0;
            }
        }
        
        //Changing delay times
        if(upFlag==1){
            upFlag=0;
            if(changecnt0<maxTime){
                changecnt0=changecnt0+6;//increment by 1sec
            }
        }
        
        if(dnFlag==1){
            dnFlag=0;
            if(changecnt0>minTime){
                changecnt0=changecnt0-6;//decrement by 1sec
            }
        }
    }
}
 
