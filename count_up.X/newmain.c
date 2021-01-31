/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 9, 2018, 10:24 AM
 */
#define _XTAL_FREQ 6000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

int count=0;
int count2=0;
int writeFlag=0;

int prev3=1;
int prev5=1;
int prev6=1;
int prev7=1;

long cnta=0;
long cntb=0;
long cntc=0;
long cntd=0;

void interrupt timer_isr(void){
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        count++;
           if(count==5){//about 250ms
               count=0;
                if(RB3==0 && prev3==0){
                    cnta++;
                }
                if(RB5==0 && prev5==0){
                    cntb++;
                }
                if(RB6==0 && prev6==0){
                    cntc++;
                }
                if(RB7==0 && prev7==0){
                    cntd++;
                }
            prev3=RB3;
            prev5=RB3;
            prev6=RB3;
            prev7=RB3;
        }
        
    }
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        count2++;
        
        if(count2==50){
            writeFlag=1;//Write to EEPROM
        }
    }
    
     
    TMR1=10000;
}
//EEPROM functions!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
unsigned char readEEPROM(unsigned char address)
{
  EEADR = address; //Address to be read
  //EECON1bits.EEPGD = 0;//Selecting EEPROM Data Memory, not available in 16F628a
  EECON1bits.RD = 1; //Initialise read cycle
  return EEDATA; //Returning data
}

void writeEEPROM(unsigned char address, unsigned char datas)
{
  unsigned char INTCON_SAVE;//To save INTCON register value
  
  //setting write parameters
  EEADR = address; //Address to write
  EEDATA = datas; //Data to write
  //EECON1bits.EEPGD = 0; //Selecting EEPROM Data Memory, not available in 16F628a
  EECON1bits.WREN = 1; //Enable writing of EEPROM
  
  //saving INTCON register
  INTCON_SAVE=INTCON;//Backup INCON interupt register
  INTCON=0; //Diables the interrupt
  
  //Writing to the EEPROM
  EECON2=0x55; //Required sequence for write to internal EEPROM
  EECON2=0xAA; //Required sequence for write to internal EEPROM
  EECON1bits.WR = 1; //Initialise write cycle
  INTCON = INTCON_SAVE;//Enables Interrupt
  EECON1bits.WREN = 0; //To disable write
  while(EEIF == 0);//writing  currently
  EEIF = 0; //Clearing EEIF bit
}

void main(void) {
    //RX=RB1 and TX=RB2 for UART
    TRISB=0b11111000;
    PORTB=0b11111000;
    
    //Timer1 setup
    TRISA=0x00;
    TRISB=0x00;
    T1CON = 0b01100001;//ps8,on
    PIE1bits.TMR1IE = 1;           
    INTCONbits.PEIE = 1;           // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE =1;
    PIR1bits.TMR1IF = 0;
    
    //timer0 setup
    OPTION_REG=0b00000111;//PS=256
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    INTE=1; 
    
    TMR1=10000;
    
    while(1){
        if(writeFlag==1){
            writeFlag=0;
            
            writeEEPROM(0,(char)cnta);
            writeEEPROM(1,(char)cntb);
            writeEEPROM(2,(char)cntc);
            writeEEPROM(3,(char)cntd);
        }
    }
}
