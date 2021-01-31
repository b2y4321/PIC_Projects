/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 6, 2019, 8:58 PM
 */
#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = INTOSCIO        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKINT)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdlib.h> //Need this for random num library

//defined RAND_MAX of 32767
int resul=0;
double pwmod=0;
int a=RAND_MAX;

//PWM parameters
int brt=0;
int brtmax=10;
int cnt=0;
int cnd=0;
int changeFlag=1;//Do we need to recalc rand nums

void main(void) {
    OSCCON=0b1100101;//Internal oscillator, 4MHz
    CMCON0=0x07; 
    ANSEL=0x00;
    
    TRISIO=0x00;
    GPIO=0x00;
    
    srand(0xFF);//random number seeding
    
    //Timer1 setup
    T1CON = 0b00110001;//PS=8,on
    PIE1bits.TMR1IE = 1;      
    INTCONbits.PEIE = 1;
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    TMR1=3036;
    
    //timer0 setup
    OPTION_REG=0x00;//PS=2
    TMR0=0;
    INTCONbits.TMR0IE = 1;  
    INTCONbits.TMR0IF=0;
    
    GP0=1; 
    
    while(1){
        if(changeFlag==1){
            changeFlag=0;
            resul=rand(); 
            pwmod=((double)resul)/((double)a);
            brt=(int)(pwmod*(double)brtmax);
        }
    }
}

void interrupt timer_isr(){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        cnt++;
        
        if(cnt>=brt){
            GP0=0;
        }
        if(cnt>=brtmax){
            cnt=0;
            GP0=1;
        }
    }
    
    //2hz signal
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        TMR1=3036;
        
            changeFlag=1;
        }
    
    
}