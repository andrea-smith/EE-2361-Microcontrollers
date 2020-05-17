// EE 2361 Final Project:
// Chrome Dino Autobot
// Andrea Smith & Ferris Henning
// Spring 2020


#include "xc.h"

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#define JUMP_OFFSET 100;
#define DISTANCE_MULTY 1;
#define PRESSED_ANGLE 45 *  (1000/90);
#define UNPRESSED_ANGLE 0 *  1000/90;

int dinoAction=2; //0=jump, 1=duck, 2=nothing
int the_next_move=2; //same as above acts like a 1 length buffer

void action(int move);

void setup_dino() {
    
     __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR0bits.INT1R = 8;
    RPINR1bits.INT2R = 9;
    RPOR3bits.RP6R = 18; // RP6 for OC1 
    RPOR2bits.RP5R = 19;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    
    CLKDIVbits.RCDIV = 0;
    //CNPU2bits.CN22PUE = 1; // Pull up resistor
    AD1PCFG = 0x9fff;
    TRISB = 0b111000000;
    TRISA = 0b0; // RA0 output
    
    
    INTCON2 = 0; // INTCON2 reg controls external interrupts
    IFS0bits.INT0IF = 0; // Clear the flag..? Idk if this is necessary
    
    // Set up ISR 0
    _INT0EP = 0; // External interrupt 0 occurs on rising edge
    _INT0IE = 1; // Enable

    // Set up ISR 1
    _INT1EP = 0;
    _INT1IE = 1; // Enable
    
    // Set up ISR 2
    _INT2EP = 0;
    _INT2IE = 1; // Enable
    
    
    /* Timer 1-3 setup are indentical */
    
    // Timer 1 setup 
    T1CON = 0;
    TMR1 = 0;
    T1CONbits.TON = 0; // Off for now
    T1CONbits.TCKPS = 0b11; // 256 prescaler
    PR1 = 0; 
    
    // Timer 2 setup
    T2CON = 0;
    TMR2 = 0;
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 0b11;
    PR2 = 0;
    
    // Timer 3 setup
    T3CON = 0;
    TMR3 = 0;
    T3CONbits.TON = 0;
    T3CONbits.TCKPS = 0b01;
    PR3 = 39999;
    _T3IF = 0;
    T3CONbits.TON = 1;
    
    // Output Compare setup
    OC1CON = 0;
    OC1R = 0;
    OC1RS = 2000;
    
    OC1CONbits.OCTSEL = 1;  // Timer 3 as clock source
    OC1CONbits.OCM = 0b101; // Continuous output pulses on OCx
    
    OC2CON = 0;
    OC2R = 0;
    OC2RS = 2000;
    
    OC2CONbits.OCTSEL = 1;
    OC2CONbits.OCM = 0b101;
    
    OC2RS = 3000 +  UNPRESSED_ANGLE;
    OC1RS = 3000 +  UNPRESSED_ANGLE;
   
}

void __attribute__((__interrupt__,__auto_psv__)) _INT0Interrupt(void) // Jump
{
    T1CONbits.TON=1;
    _INT0IF = 0;
    dinoAction=0;
    }

void __attribute__((__interrupt__,__auto_psv__)) _INT1Interrupt(void) // Duck
{
    if(T1CONbits.TON == 0){
        dinoAction=1;
    }
    T1CONbits.TON=1;
    _INT1IF = 0;
}

void __attribute__((__interrupt__,__auto_psv__)) _INT2Interrupt(void) // When to 
{
    if(T1CONbits.TON==1){
    PR2= TMR1 * DISTANCE_MULTY  - JUMP_OFFSET;
    T2CONbits.TON=1;
    T1CONbits.TON=0;
    TMR1 = 0;
    the_next_move=dinoAction;
    dinoAction=3;
    OC2RS = 3000 +  UNPRESSED_ANGLE;
    OC1RS = 3000 +  UNPRESSED_ANGLE;
    }
    _INT2IF = 0;
}

void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void) // When to jump
{
    action(the_next_move);
    IFS0bits.T2IF = 0;
    the_next_move=3;    
    
}

void action(int move){
    if(move==0){
        OC1RS = 3000 +  PRESSED_ANGLE; // Jump 
    } else{
        OC2RS = 3000 +  PRESSED_ANGLE;
    }
}

