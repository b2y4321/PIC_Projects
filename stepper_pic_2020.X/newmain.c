/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on March 29, 2020, 10:59 PM
 */
#define _XTAL_FREQ 16000000

#define pin1 RB0
#define pin2 RB1
#define pin3 RB2
#define pin4 RB3
#define up RB5
#define dn RB6
#define rst RB7


// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

void incrMotor();
//28BYJ-48: 2000 steps per rev
int pwrmin=1;
int pwrmax=16;
int pwr=5;
int pwrinit;

int stepnum=0;
int stepflag=0;
int recalcFlag=0;

int prevup=1;
int prevdn=1;
int prevrst=1;
long t0val=0;
int cnt=0;

void __interrupt timer_isr(void){
    //stepper exec
    if(INTCONbits.TMR0IF==1){ 
        INTCONbits.TMR0IF=0;
        TMR0=t0val;
        stepflag=1;
    }
    
    
    //debounce regime 3.81hz
    if(PIR1bits.TMR1IF ==1){
        PIR1bits.TMR1IF = 0;
        cnt++;
         
        if(cnt>=10){
            cnt=0;
            
            if(prevup==0 && up==0){
                if(pwr<pwrmax){
                    pwr++;
                    recalcFlag=1;
                }
            }
            if(prevdn==0 && dn==0){
                if(pwr>pwrmin){
                    pwr--;
                    recalcFlag=1;
                }
            }
            if(prevrst==0 && rst==0){
                pwr=pwrinit;
                recalcFlag=1;
            }

            prevup=up;
            prevdn=dn;
            prevrst=rst;
        }
        
        
        
    }
}

void main(void) {
    pwrinit=pwr;
    
    TRISA=0x00;
    PORTA=0x00;
    TRISB=0b11100000;
    PORTB=0b11100000;
    
    //timer setup
    //Timer1 setup
    T1CON = 0b00010001;//timer1, ps8, turn it on
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    TMR1=0;
    
     
    //timer0 setup
    OPTION_REG=0b00000110;//PS=128, minimum 61 full cycles/sec
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    
    //TMR0=0;
    t0val=(long)(230.0*((double)pwr/(double)pwrmax));
    TMR0=t0val;
    
    while(1){
        if(stepflag==1){
            stepflag=0;
            incrMotor();
        }
        if(recalcFlag==1){
            recalcFlag=0;
            t0val=(long)(230.0*((double)pwr/(double)pwrmax));
        }
    }
    
}



void incrMotor(void){
    if(stepnum==0){
            pin1=1;
            pin2=0;
            pin3=0;
            pin4=0;
            
            stepnum=1;
        }
    else if(stepnum==1){
            pin1=1;
            pin2=1;
            pin3=0;
            pin4=0;
            
            stepnum=2;
        }
    else if(stepnum==2){
            pin1=0;
            pin2=1;
            pin3=0;
            pin4=0;
            
            stepnum=3;
        }
    else if(stepnum==3){
            pin1=0;
            pin2=1;
            pin3=1;
            pin4=0;
            
            stepnum=4;
        }
    else if(stepnum==4){
            pin1=0;
            pin2=0;
            pin3=1;
            pin4=0;
            
            stepnum=5;
        }
    else if(stepnum==5){
            pin1=0;
            pin2=0;
            pin3=1;
            pin4=1;
            
            stepnum=6;
        }
    else if(stepnum==6){
            pin1=0;
            pin2=0;
            pin3=0;
            pin4=1;
            
            stepnum=7;
        }
    else{ //step 7
            pin1=1;
            pin2=0;
            pin3=0;
            pin4=1;
            
            stepnum=0;
        }
}

