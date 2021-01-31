/*
 * File:   newmain.c
 * Author: Brian
 *
 * Created on December 19, 2019, 1:07 PM
 */


#include <xc.h>
//First traffic light specs
int tgarr1=5;
int tyarr1=2;
int tgrn1=10;
int tyel1=2;
int tsw1=2;

//2nd traffic light specs
int tgarr2=5;
int tyarr2=2;
int tgrn2=10;
int tyel2=2;
int tsw2=2;
int coord12=0;//Do u want to coordinate 1 and 2? If so, 2nd light will start
//sequence when the turn arrow of first light expires, without arrows

//3rd traffic light specs
int tgarr3=5;
int tyarr3=2;
int tgrn3=10;
int tyel3=2;
int tred3=2;

//Non-user changeable specs
//timer counters
int debcount=0;
int secticker=0;
int sec=0;
int flashcount=0;

//generic flags
int stgnum=0;

//arrow flasher indicators flags
int grn1arr=0;
int yel1arr=0;
int grn2arr=0;
int yel2arr=0;
int grn3arr=0;
int yel3arr=0;

void interrupt timer_ISR(void)             // High priority interrupt
{
    if (PIR1bits.TMR1IF == 1)
    { 
        PIR1bits.TMR1IF=0; 
        
    }
    
    if(INTCONbits.TMR0IF==1){
        INTCONbits.TMR0IF=0; 
        flashcount++;
        
        if(grn1arr==1){
            
        }
        if(yel1arr==1){
            
        }
        if(grn2arr==1){
            
        }
        if(yel2arr==1){
            
        }
        if(grn3arr==1){
            
        }
        if(yel3arr==1){
            
        }
        
    } 
}    

void main(void) {
    TRISA=0x00;
    TRISB=0x00;
    TRISC=0b00000111;
    TRISD=0x00;
    TRISE=0x00;
    
    PORTA=0x00;
    PORTB=0x00;
    PORTC=0b00000111;
    PORTD=0x00;
    PORTE=0x00;
    
    //Timer1 setup - for debounce and timing
    T1CON = 0b00110001;//timer1, turn it on
    PIE1bits.TMR1IE = 1;//timer1       
    INTCONbits.PEIE = 1;//timer0,1
    INTCONbits.GIE =1;//timer0,1
    PIR1bits.TMR1IF = 0;//timer1  
    
    TMR1=3036;//Designed with PS for 6hz pulse
    
     
    //timer0 setup - for flashing (representing arrows))
    OPTION_REG=0b00000111;//PS=256, resulting in 46hz signal (divide by 12 in ISR))
    TMR0=0;//timer0 count (8 bit)
    INTCONbits.TMR0IE = 1; //timer0  
    INTCONbits.TMR0IF=0; //timer0
    //There is really no way to turn on off timer0
    
}
