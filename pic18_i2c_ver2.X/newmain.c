/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 13, 2020, 11:59 PM
 */

#define _XTAL_FREQ 16000000
#define addr 0x68

// CONFIG1H
#pragma config FOSC = IRC       // Oscillator Selection bits (Internal RC oscillator)
#pragma config PLLEN = OFF      // 4 X PLL Enable bit (PLL is under software control)
#pragma config PCLKEN = ON      // Primary Clock Enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 19        // Brown Out Reset Voltage bits (VBOR set to 1.9 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up bit (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled, RA3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = OFF      // Boot Block Size Select bit (1kW boot block size)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

void I2Cidle(){
    while (( SSPCON2 & 0x1F ) || ( SSPSTATbits.R_nW));
}
void I2CStart() //request a data update from the slave(s)
{
    SEN = 1; //sart sequence
    while (SEN == 1); //wait until it finished
    
    while(!SSPIF); //It's getting stuck here
    SSPIF=0;
    
    //I2Cidle(); //didn't need, according to a person
}

void I2CWrite(int dat){
    SSPBUF = dat; //write bit
    CKP=1;
    
    while(!SSPIF);
    SSPIF=0;
    while (BF == 1); //wait while this is sended
    while (ACKSTAT == 1); //wait until the ack is done
    
    //Acknowledge sequence
    ACKDT = 0; //not sure....... AN735 doc
    ACKEN = 1; // (should be the sign for the slave to go on transmitting)
    
    //I2Cidle();
}

void I2CStop(){
    PEN = 1; //stop sequence
    while(PEN);
}
void I2CRestart(){
    RSEN = 1; //stop sequence
    while(RSEN);
    
    //I2Cidle();
}

void main(void) //main routine to manage everything
{
    OSCCONbits.IRCF = 0b111; //16MHz

    SSPCON1bits.SSPEN = 1; //enable ssp
    SSPCON1bits.SSPM = 0b1000; //set as master
   

    SSPADD = 39; //100kHz

    TRISBbits.RB4 = 1; //set as inputs
    TRISBbits.RB6 = 1; 
    
    I2CStart();
    I2CWrite(addr << 1);//def need this, othw NACK
    I2CWrite(0x00);
    I2CWrite(0x00);
    
    I2CWrite(addr << 1);//def need this, othw NACK
    I2CWrite(0x07);
    I2CWrite(0b10010000);
    I2CStop();
    
    while (1);
}