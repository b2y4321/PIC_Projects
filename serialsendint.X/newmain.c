#define _XTAL_FREQ  4000000
#define BAUDRATE 9600

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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



// Function that initializes UART.
void initUART(void) {

    // First calculate check if we have a high baudrate.
    

        // Define pins as output
        unsigned int x=5;
        
        
        // Define UART setup.
        SPBRG = x;          // Writing SPBRG Register 
        BRGH=0;
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

// Function that sends strings through serial port.

// Main function.
void main(void) {
    TRISB=0b00000110;
    PORTB=0x00;
    nRBPU = 0; // Enables PORTB Internal Pull Up Resistors
    // Initialize UART.
    initUART();
    while(1) {

        // Send string through UART.
        sendData(224);
        __delay_ms(300);
        RB5=1;
        __delay_ms(200);
        RB5=0;
    }
}