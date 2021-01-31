/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on June 8, 2018, 11:43 PM
 */
#define _XTAL_FREQ 12000000
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

//BUTTONS:
//RB4 down 
//RB5 up
//RB0 ZDC
//RB3 gate trigger

double pwr=5.0;//1 to 20 

int t1assign=0;//assignment for timer1 based on power
int t1recalc=0;//Do we need to recalculate power values?
int ZC=0;

unsigned int charcounter=0;
char cbuf [2];

int baudrate=9600;
void initUART(void) {
        
        unsigned int x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
        // Define UART setup.
        SPBRG = x;          // Writing SPBRG Register 
        BRGH=1;//good to set at high baudrate
        SYNC = 0;                                     // Setting Asynchronous Mode, ie UART
        SPEN = 1;                                     // Enables Serial Port
        CREN = 1;                                     // Enables Continuous Reception
        TXIE  = 0;                                    // Disable tx interrupts
        //RCIE  = 1;                                    // Enable rx interrupts
        RCIE = 0;
        TX9   = 0;                                    // 8-bit transmission
        RX9   = 0;                                    // 8-bit reception
        TXEN  = 0;                                    // Reset transmitter
        TXEN = 1;                                     // Enables Transmission
    
}

// Function that sends characters through serial port.
void sendData(int data) {
   while(!TRMT);  // wait for previous transmission to finish
   TXREG = data;
   
}

char UART_Read()
{
  while(!RCIF);
  return RCREG;
}

// Function that sends strings through serial port.
void interrupt timer_ISR(void)            
{ 
    
    if(INTF==1){
        INTF=0;
        ZC=1;
    }
    if (PIR1bits.TMR1IF == 1){ 
        PIR1bits.TMR1IF=0; 
        
        if(RB3==0){
            //fire the gates
            T1CON=0x00;
            RB3=1;
            __delay_us(650);
            RB3=0;
        }
    }  
} 

void main(void) {
    TRISB=0b00000001;
    PORTB=0b00000001;
    //Timer1 setup
    T1CON = 0x00;//off
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    //RB0 interr setup
    INTE=1;
    INTF=0;//ext interrupt flag
    INTEDG=1;//rising edge
    
    RB3=0;
    t1assign=(53036.0+(65535.0-53036.0)*(pwr/20.0));
    
    //UART initialization
    nRBPU = 0; // Enables PORTB Internal Pull Up Resistors
    initUART();
    
    //sendData(224);
    while(1){
        if(ZC==1){
            ZC=0;
            TMR1=t1assign;
            T1CON = 0b00000001;//ps1
        } 
        //Reading power levels
        if(!RCIF){
            char a=UART_Read();
            charcounter++;
            cbuf[charcounter-1]=a;
            if(charcounter==2){
                pwr=(double)((cbuf[0]*10-'0')+(cbuf[1]-'0'));
                sendData((int)pwr);
            }
        }
    }
    }

