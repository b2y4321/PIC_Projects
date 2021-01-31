/*
 * File:   newmain.c
 * Author: Briann
 *
 * Created on December 29, 2019, 2:05 PM
 */
#define addr 0x27
#define up RA5
#define dn RA4
#define third RA0

#include <xc.h>

unsigned int cnt=0;
int prevup=1;
int prevdn=1;
int prev3=1;
int b3cnt=0;

int secStage=0;//post-press debounce timer
int changeFlag=0;


//RA2 is interrupt pin
//RA1 and RA2 are clock/data
void __interrupt () timer_isr (void){
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0;
        
        //debounce second bit clear
        if(secStage!=0){
            secStage++;
        }
        if(secStage==3){
            secStage=0;//if after-debounce timer expires, reset to 0 and accept presses again
        }
        
        if(prevup==0 && RA5==0){
            if(secStage==0){
                cnt++;
                changeFlag=1;
                secStage=1;
            }
            
        }
        if(prevdn==0 && RA4==0){
            if(cnt>0 && secStage==0){
                cnt--;
                changeFlag=1;
                secStage=1;
            }
        }
    }
    
}

void main(void) {
    ANSELA=0x00;//All digital inputr
    OSCCON=0b01011011;//1Mhz internal clock
    
    TRISA=0b110111;
    PORTA=0b110111;
    
    //Timer0 setup
    OPTION_REG=0xb00000111;//PS of 256, timer0 is always on. 3.81hz
    INTCONbits.TMR0IE=1;//timer0
    INTCONbits.PEIE=1;//tmr0,1
    INTCONbits.GIE=1;//tmr0,1
    INTCONbits.TMR0IF=0;//timer0 interrupt
    
    while(1){
        if(changeFlag==1){
            changeFlag=0;
            displayNumber(cnt);
        }
    }
    
    
}

void displayNumber(int num){
    //set cursor at position 0
    sendData(0x80);
    
    
}
void sendData(int cmds){
    
}
int startI2C(void){
    SSP1CON2bits.SEN=1;
    while(!SSP1IF);
    SSP1IF=0;
    SSP1BUF=addr;
    while(!SSP1IF);
    SSP1IF=0;

    return ACKSTAT;
}