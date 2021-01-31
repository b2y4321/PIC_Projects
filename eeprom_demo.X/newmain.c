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
  INTCON  
 = INTCON_SAVE;//Enables Interrupt
  EECON1bits.WREN = 0; //To disable write
  while(EEIF == 0);//writing  currently
  EEIF = 0; //Clearing EEIF bit
}

unsigned char a;
void main(void) {
    TRISB=0x00;
    PORTB=0x00;
    
    while(1){
    //first write cycle
    RB1=1;
    writeEEPROM(0,(char)RB1);
    __delay_ms(2000);
    a=readEEPROM(0);
    RB2=(int)a;
    __delay_ms(2000);
    RB1=0;
    RB2=0;
    
    //2nd write cycle
    RB2=1;
    writeEEPROM(50,(char)RB2);
    __delay_ms(2000);
    a=readEEPROM(50);
    RB1=(int)a;
    __delay_ms(2000);
    RB1=0;
    RB2=0;
    
    //first write cycle
    RB1=1;
    writeEEPROM(100,(char)RB1);
    __delay_ms(2000);
    a=readEEPROM(100);
    RB3=(int)a;
    __delay_ms(2000);
    RB1=0;
    RB3=0;
    
    //first write cycle
    RB3=1;
    writeEEPROM(115,(char)RB3);
    __delay_ms(2000);
    a=readEEPROM(115);
    RB1=(int)a;
    __delay_ms(2000);
    RB1=0;
    RB3=0;
    }
}
