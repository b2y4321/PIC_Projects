/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on December 10, 2018, 10:48 PM
 */

#define _XTAL_FREQ 32768

#pragma config FOSC = LP  
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#include <xc.h>


int ctr=0;
int pwrmax=10;
int pwr=6;

int pwri=0;
void interrupt timer1_isr(void){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        ctr++;
         
        if(ctr>=pwri){ 
            GP2=0; 
        }
        if(ctr>=pwrmax){
            ctr=0;
            GP2=1;
        }
        
    }
}
void main(void) {
    pwri=pwrmax-pwr;
    TRISIO=0x00;
    
    OPTION_REG=0b00000100;//PS=32 for 1 hz pulse
    TMR0=0;
    INTCONbits.TMR0IE = 1; 
    INTCONbits.PEIE = 1;            
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IF=0; 
    
    GP2=1;
    while(1);

}
