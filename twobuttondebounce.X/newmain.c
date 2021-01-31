/*
 * File:   newmain.c
 * Author: Briann
 *
 * Created on December 31, 2019, 11:33 AM
 */
#define b1 GP0
#define b2 GP1
#define rly GP2

#include <xc.h>

int prev1=1;
int prev2=1;
int pressflag=0;

void __interrupt () timer_isr (void){
    //After a successful button press, we ignore button presses for 2 debounce cycles
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF=0;
        
        if(pressflag>=2){
            pressflag=0;
        }
        
        if(pressflag==1){
            pressflag++;
        }
        
        if(prev1==0 && b1==0 && pressflag==0){
            rly=~rly;
            pressflag=1;
        }
        if(prev2==0 && b2==0 && pressflag==0){
            rly=~rly;
            pressflag=1;
        }
    }
}

void main(void) {
    CMCON0=0x07;
    ANSEL=0x03;
    TRISIO=0b000011;
    GPIO=0b000011;
    
    //Timer1 setup
    T1CON=0b00110001;//ps of 8, on
    INTCONbits.PEIE=1;//tmr0,1
    INTCONbits.GIE=1;//tmr0,1
    PIE1bits.TMR1IE=1;//tmr1 only
    PIR1bits.TMR1IF=0;//Timer1 interrupt flag
    
    rly=0;
}
