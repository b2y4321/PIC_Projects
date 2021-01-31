/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on February 4, 2018, 7:07 PM
 */


#include <xc.h>


#include <libpic30.h>
#include <p30f4011.h>
 
// Configuration settings
_FOSC(CSW_FSCM_OFF & FRC_PLL16); // Fosc=16x7.5MHz, Fcy=30MHz
_FWDT(WDT_OFF);                  // Watchdog timer off
_FBORPOR(MCLR_DIS);              // Disable reset pin
 
// Function prototype for timer 1 ISR
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
 
int main()
{
    // Configure RD0 and RD1 as outputs
    TRISD = 0b1100;
 
    // Configure Timer 1.
    // PR1 and TCKPS are set to call interrupt every 500ms.
    // Period = PR1 * prescaler * Tcy = 58594 * 256 * 33.33ns = 500ms
    T1CON = 0;            // Clear Timer 1 configuration
    T1CONbits.TCKPS = 3;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR1 = 58594;          // Set Timer 1 period (max value is 65535)
    _T1IP = 1;            // Set Timer 1 interrupt priority
    _T1IF = 0;            // Clear Timer 1 interrupt flag
    _T1IE = 1;            // Enable Timer 1 interrupt
    T1CONbits.TON = 1;    // Turn on Timer 1
 
    // Main loop just flashes LED on RD0 at approx 2Hz.
    // Meanwhile, Timer 1 ISR flashes LED on RD1 at 1Hz.
    while(1)
    {
        _LATD0 = 1;
        __delay32(7500000);
        _LATD0 = 0;
        __delay32(7500000);
    }
 
    return 0;
}
 
// Timer 1 interrupt service routine toggles LED on RD1
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    // Clear Timer 1 interrupt flag
    _T1IF = 0;
 
    // Toggle LED on RD1
    _LATD1 = 1 - _LATD1;
}