/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 13, 2020, 5:47 PM
 */

#define addr 0x68

#define _XTAL_FREQ 16000000

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

int acks1=1;
int acks2=1;
int acks3=1;
int acks4=1;

int badFlag=0;

void i2c_idle(void)
{
  // Wait I2C Bus and Status Idle (i.e. ACKEN, RCEN, PEN, RSEN, SEN)
  while (( SSPCON2 & 0x1F ) || ( SSPSTATbits.R_nW));
}

void I2CStart(){
    //Initializing I2C mode (Master)
    SSPCON1bits.SSPEN=1;
    
    SSPCON1bits.SSPM3=1;
    SSPCON1bits.SSPM2=0;
    SSPCON1bits.SSPM1=0;
    SSPCON1bits.SSPM0=0;
    
    //Start I2C
    i2c_idle();
    SEN=1;
    
    while(SEN);
    
    acks1=ACKSTAT;
}

void transmitData(char dat){
    i2c_idle();
    SSPBUF=dat;
    
    while(BF);
    
    acks2=ACKSTAT;
    if(acks2==1){
        badFlag=1;
        return;
    }
}

void transmitData1(char dat){
    i2c_idle();
    SSPBUF=dat;
    
    while(BF);
    
    acks3=ACKSTAT;
    if(acks3==1){
        badFlag=1;
        return;
    }
}

void I2CStop(){
    PEN=1;
        
    while(PEN);
    
    acks4=ACKSTAT;
}

void I2CRestart(){
    SSPCON2bits.RSEN=1;
    while(RSEN);
}



void main(void) {
    //Setting up
    OSCCONbits.IRCF2=1;
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF0=1;
    
    TRISA=0x00;
    TRISB=0x00;
    TRISC=0x00;
    TRISBbits.RB4=1;
    TRISBbits.RB6=1;
    
    PORTA=0x00;
    PORTB=0x00;
    PORTC=0x00;
    
    
    SSPADD=39;//I2C freq is Fosc/(4*(SSPADD+1))
    //50khz
    
    //Initialize connection on DS1307
    I2CStart();
    transmitData(addr); //attach 0 to signify write
    transmitData1(0x00); 
    transmitData1(0x00);
    
    transmitData1(0x07);
    transmitData1(0x10);
    I2CStop();
    
    while(1);
}
