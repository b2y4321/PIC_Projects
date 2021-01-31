/*
 * File:   newmain1.c
 * Author: Briann
 *
 * Created on January 2, 2020, 2:24 PM
 */
#define _XTAL_FREQ 8000000

#define lt GP2

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
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

int lsvals[14]={15,14,12,10,8,6,5,4,5,6,8,10,12,14};//4 bit width, not 8 bit
double tswitch=0.1;//Switch time, anything less than 262ms

//PWM parameters
int pwrlvl=0;
int tmax=16;
int pwmcnt=0;

//Mode change parameters
int chngFlag=0;
int arrI=0;//current array index 
int szArr=0;//array size

//Vars set aside for timer value calc

double f1=0;
unsigned int t1val=0;

void __interrupt () timer_isr (void){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        pwmcnt++;
        if(pwmcnt>=tmax){
            pwmcnt=0;
            lt=1;
        }
        if(pwmcnt>pwrlvl){
            lt=0;
        }
    }
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF=0;
        TMR1=t1val;
        chngFlag=1;
    }
}

void main(void) {
    CMCON0=0x07;
    ANSEL=0x03;
    
    TRISIO=0x00;
    GPIO=0x00;
    
    //Timer0 setup - pwm
    OPTION_REG=0x01;//PS of 4, giving 3906hz interrupt rate
    INTCONbits.TMR0IE=1;//timer0
    INTCONbits.PEIE=1;//tmr0,1
    INTCONbits.GIE=1;//tmr0,1
    INTCONbits.TMR0IF=0;//timer0 interrupt
    
    
    //Timer1 setup - modeswitch
    T1CON=0b00110001;//ps of 8, on
    PIE1bits.TMR1IE=1;//tmr1 only
    PIR1bits.TMR1IF=0;//Timer1 interrupt flag
    
    szArr=sizeof(lsvals)/sizeof(lsvals[0]);
    pwrlvl=lsvals[0];
    arrI++;
    
    lt=0;
    
    //Timer value calculation
    f1=(1.0/tswitch)/3.81*65535.0;
    t1val=65535-(unsigned int)f1;
    
    while(1){
        if(chngFlag==1){
            //switch to another brightness val
            chngFlag=0;
            pwrlvl=lsvals[arrI];
            arrI++;
            
            //reset index if we reach end
            if(arrI==szArr){
                arrI=0;
            }
        }
    }
}
