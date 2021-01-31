/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on November 6, 2019, 6:34 PM
 */
#define _XTAL_FREQ 4000000
#define addr1 0x68
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int num=0;
int aw=0;
int ackst=1;//ACK bit status
unsigned int charbuf1=0;
 

void I2C_Master_Init(unsigned int addr)
{
    SSP1CON1=0b00001000;//Master mode, Clock determined by Fosc/(4(SSPADD+1)))
    
    SSP1CON2bits.SEN=1;//Start transmit
    while(!SSP1CON2bits.SEN);//wait for complete start
    //PIR1bits.SSP1IF=1;//clear start flag
    //MSSP1 waits here
    SSP1BUF=addr;
    ackst=SSP1CON2bits.ACKSTAT;
    
    aw++;
}
void I2CWrite(unsigned int c){
    //while ((SSPSTAT && 0x04) || (SSPCON2 && 0x1F));
    ackst=1;
    
    //WCOL is internal?
    //SSP1IF is internal?
    
    //while(SSP1CON1bits.WCOL); //internal flag
    //PIR1bits.SSP1IF=0;
    SSP1BUF=c;//load slave addr. Transmit happens automatically after
    //while(PIR1bits.SSP1IF);
    ackst=SSP1CON2bits.ACKSTAT;//ACK command, 1 or 0
    
    aw++;
}
char I2CRead(void){
    //To be called right after a transmit cmd
    SSP1CON2bits.RCEN=1;//Enable receive
    while(!PIR1bits.SSP1IF);//After received byte, flag is set here
    PIR1bits.SSP1IF=0;//clear SSP1 flag
    
    while(!SSP1STATbits.BF);//Waitw for buffer to be filled
    charbuf1=SSP1BUF;//Load buffer from received byte register
    SSP1STATbits.BF=0;//Clear buffer flag
     
    //Wait for acknowledge
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0;
    
    return charbuf1;
}
void I2CStop(){
    SSP1CON2bits.PEN=1;//Stop command, cleared auto by hardware
    while(!SSP1CON2bits.PEN);
    
    aw++;
}
void I2CRestart(){
    SSP1CON2bits.RSEN=1;//Restart command. Auto cleared by hardware
    while(!SSP1CON2bits.RSEN);
    
    aw++;
}

void main(void) {
    ANSELA=0x00;
    //OSCCON=0b01101011;//Internal oscillator, 4MHz
    SSPADD=19;//controls clock freq by formula below
    
    //setting IO bits. SCL is RA1, SDA is RA2
    TRISA=0b110110;
    //SDA and SCL are instructed to be set as inputs
    
    PORTA=0b110110;
    
    PIE1bits.SSP1IE=1;       // Synchronous Serial Port (MSSP) Interrupt Enable bit
    SSP1CON3bits.SCIE=1;     //Start Condition Interrupt Enable bit
    SSP1CON3bits.PCIE=1;     //Stop Condition Interrupt Enable bit
    
    
    //Counting from 1 to 9
    
    
        I2C_Master_Init(addr1);
        
        I2CWrite(0xD0);
        I2CWrite(0x00);//dig1
        I2CWrite(0x00);//dig2
        
        
        I2CRestart();
        //I2CRestart();//Apparently do this instad of stop and start
        
        I2CWrite(0xD0);
        I2CWrite(0x07);//dig1
        I2CWrite(0b00010000);//dig2
        
         
        I2CStop();
    
    
    
      
        
        /*
        I2CWrite(0xC0);//digit register
        I2CWrite(0x00);//dig1
        I2CWrite(0x00);//dig2
        I2CWrite(0x00);//dig3
        I2CWrite(num);//dig4
         */

    while(1){
         
    }
}

