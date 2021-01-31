/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on April 6, 2019, 8:17 PM
 */
#define _XTAL_FREQ 8000000

#define red GP0
#define yel GP1
#define grn GP2
#define ga GP4 //grn arrow
#define ya GP5 //yel arrow

// CONFIG
#pragma config FOSC = INTOSCIO        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN)
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

//Time btwn steps
int timestep[5]={10,2,10,3,15};

int stepnum=1;
//1 is grn+grn arrow
//2 is grn+yellow arrow
//3 is grn only
//4 is yellow only
//5 is red only

int cnt=0;
int sec=0;
int keygen=1;//startup process flag

void main(void) {
    OSCCON=0b1110101;//Internal oscillator, 8MHz
    CMCON0=0x07;
    ANSEL=0x00;
     
    
    TRISIO=0x00;
    GPIO=0x00;
     
    //Timer1 setup
    T1CON = 0b00110001;//PS=8,on
    PIE1bits.TMR1IE = 1;   
    INTCONbits.PEIE = 1;
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    //int a=10;
    TMR1=3036;
    
    //Initial Settings
    red=0;
    yel=0;
    grn=1;
    ya=0;
    ga=1;

    
    while(1){
        
        if(cnt==4){
            cnt=0;
            sec++;
        }
        if(sec>=timestep[stepnum-1]){
                
            if(stepnum==5){
                stepnum=1;
                sec=0;
                keygen=1;//To prevent double incrementing
            }
            if(stepnum<=4 && keygen==0){
                stepnum++;
                sec=0;
            }
            keygen=0;//Reset!
            
            
            if(stepnum==1){
                red=0;
                yel=0;
                grn=1;
                ya=0;
                ga=1;
            }
            if(stepnum==2){
                red=0;
                yel=0;
                grn=1;
                ya=1;
                ga=0;
            }
            if(stepnum==3){
                red=0;
                yel=0;
                grn=1;
                ya=0;
                ga=0;
            }
            if(stepnum==4){
                red=0;
                yel=1;
                grn=0;
                ya=0;
                ga=0;
            }
            if(stepnum==5){
                red=1;
                yel=0;
                grn=0;
                ya=0;
                ga=0;
            }
            
        }
        
    }
}

void interrupt timer_isr(){
    
    //4hz signal
    if(PIR1bits.TMR1IF==1){
        PIR1bits.TMR1IF = 0;
        TMR1=3036;
        
        cnt++;
    }
}
