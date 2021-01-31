/*
 * File:   newmain.c
 * Author: Briann
 *
 * Created on December 28, 2019, 5:23 PM
 */
#define gate1 RB1
#define gate2 RB2

#include <xc.h>

int pwrlvl=50;
int pwrmax=100;
int pwrmin=1;

int changeFlag=1;
unsigned int t1val=0;
int gatefire=0;
void __interrupt () timer_isr (void){
    //zdc interrupt
    if(INTF==1){
        INTF=0;
        TMR1=t1val;
        T1CON=0x01;//PS1, on
    }
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        
    }
    
    //phase control fire system
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF=0;
        if(gatefire==0){
            gatefire=1;
            TMR1=65516;
            gate1=1;
            gate2=1;
        }
        if(gatefire==1){
            gatefire=0;
            gate1=0;
            gate2=0;
            T1CON=0x00;
        }
    }
}

void main(void) {
    TRISB=0b01100001;//6,7 buttons, and 0 is zdc interrupt
    //Timer0 setup
    OPTION_REG=0b00000010;//PS of 2, timer0 is always on
    INTCONbits.TMR0IE=1;//timer0
    INTCONbits.PEIE=1;//tmr0,1
    INTCONbits.GIE=1;//tmr0,1
    INTCONbits.TMR0IF=0;//timer0 interrupt
    
    //Timer1 setup for debounce
    T1CON=0x00;//ps of 1, off
    PIE1bits.TMR1IE=1;//tmr1 only
    PIR1bits.TMR1IF=0;//Timer1 interrupt flag
    
    //Ext interrupt
    INTE=1;
    INTF=0;
    
    while(1){
        //recalc the system timer val. Based on power level, 20mhz crystal and Prescaler=1 for tmr1
        if(changeFlag==1){
            changeFlag=0;
            t1val=(unsigned int)(65536.0-10416.67*2.0*(1.0-((double)pwrlvl/(double)pwrmax)));
        }
        
    }
}

