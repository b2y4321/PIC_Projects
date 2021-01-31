/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on October 22, 2020, 12:41 AM
 */
// CONFIG
#define _XTAL_FREQ 4000000

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

//setting for 3.3khz
int pr2_timer2=30;
int prescalert2=4;//1,4,16
int pwmval=50;

int ckpsbits;
void main(void) {
    //4MHz oscillator
    
    PR2=pr2_timer2;//This is where timer2 turns over (it count from 0 to PR2)
    
    TRISIO=0x00;
    GPIO=0x00;
    GP4=1;
    //oscillator setup: For 25khz CCP1 signal
    TMR2IF=0;
    
    CCPR1L=pwmval-(pwmval & 0b11);//duty cycle 8 msb
    
    CCP1CON=0b00101111;//PWM mode on CCP1 pin
    CCP1CONbits.DC1B=(pwmval & 0b11);
    
    T2CON=0x00;
    
    //prescaler determination
    if(prescalert2==1){
        ckpsbits=0;
    }
    else if(prescalert2==4){
        ckpsbits=1;
    }
    else if(prescalert2==16){
        ckpsbits=3;
    }
    else{
        return;
    }
    
    T2CONbits.T2CKPS=ckpsbits;
    
    //turn timer2 on
    T2CONbits.TMR2ON=1;
    
    
    while(1){
        //flashy
        GP4=1;
        __delay_ms(500);
        GP4=0;
        __delay_ms(500);
    }
}

