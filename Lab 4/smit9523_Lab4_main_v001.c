/*
 * File:   smit9523_lab4_main_v001.c
 * Author: Owner
 *
 * Created on March 7, 2020, 8:32 PM
 */


#include "xc.h"
#include "Servo.h"

#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void setup(void) {
    CLKDIVbits.RCDIV = 0; //clock setup thing
    AD1PCFG = 0x9fff; //sets all pins to digital I/O
    TRISBbits.TRISB6 = 0; //servo pin output
    TRISBbits.TRISB8 = 1; //button pin input
    
    initServo();
}

void delay(unsigned int milli)//1s
{
 int a;
    for (a = 0; a < milli; a++) {
        asm("repeat #15993");
        asm("nop");
    }  
 return;
} 

int main(void) {
    setup();
    while(1){
        setServo(5);
        delay(1000);
        setServo(15);
        delay(1000);
    }
    return 0;
}