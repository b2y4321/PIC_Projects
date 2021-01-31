/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on December 2, 2018, 12:43 AM
 */
#define _XTAL_FREQ 12000000
#define SDA RB1
#define SCL RB2
#define hbd 500

#pragma config FOSC = HS  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

int sseg[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void writeByte(unsigned char val){
    for(int i=0;i<8;i++){
        SCL=0;
        __delay_us(hbd/2);
        if((val<<i)&(0x80)){
            SDA=1;
        }
        else{
            SDA=0;
        }
        __delay_us(hbd/2);
        SCL=1;
        __delay_us(hbd);
    }
    
    //ACK
    SCL=0;
    SDA=1;
    __delay_us(hbd);
    SCL=1;
    __delay_us(hbd);
}


void start(){
    SCL=1;
    SDA=1;
    __delay_us(hbd);
    
    SDA=0;
    __delay_us(hbd);
}
void stop(){
    SCL=0;
    SDA=0;
    __delay_us(hbd);
    
    SDA=1;
    __delay_us(hbd);
}

void setBrtness(int brtn){
    start();
    writeByte(brtn);//Brightest setting
    stop();
}
void writeDisp(int d3,int d2, int d1, int d0){
    start();
    writeByte(0x40);
    stop();
    
    start();
    writeByte(0xC0);
    writeByte(d0);
    writeByte(d1);
    writeByte(d2);
    writeByte(d3);
    stop();
}
void eraseScr(){
    writeDisp(0x06,0x5b,0x4F,0x00);
}

void main(void) {
    TRISB=0b00000110;
    PORTB=0x00;
    
    
    setBrtness(0x8F);
    eraseScr();
    while(1){
        writeDisp(sseg[1],sseg[7],sseg[3],sseg[6]);
        __delay_ms(1000);
        writeDisp(sseg[2],sseg[2],sseg[1],sseg[3]);
        __delay_ms(1000);
    }
}
