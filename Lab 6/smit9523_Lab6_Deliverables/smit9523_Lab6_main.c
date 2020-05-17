

#include "stdint.h"
#include <stdio.h>
#include "xc.h"
#include "smit9523_LCD.h"
#include "smit9523_Buffer.h"

// CW1:
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
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFE; // AN0 set as analog
    TRISA |= 1;
    
    I2C2CONbits.I2CEN = 0; // Good practice to disable before changing BRG
    I2C2BRG = 0x9D; // Value for 16 MHz
    IFS3bits.MI2C2IF = 0; // Clear interrupt, just in case
    I2C2CONbits.I2CEN = 1;
    
    initBuffer();
    lcd_init();
    lcd_setCursor(0,0);
    
    AD1CON1 = 0; // A/D control reg 1; Off for now
    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b010; // Timer 3 compare match (clk divider for fixed sample rate)
    AD1CON1bits.ASAM = 1; // Sample begins immediately after last conversion completes; SAMP auto reset
    
    
    AD1CON2 = 0; // A/D control reg 2
    AD1CON2bits.VCFG = 0b0000; // Reference voltage
    
    AD1CON3 = 0; // A/D control reg 3
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 0b1;
    AD1CON3bits.ADCS = 0b1; // 2 * Tcy
    
    AD1CON1bits.ADON = 1; // Turn on module
    
    AD1CHS = 0; // A/D input channel select register
    AD1CHSbits.CH0NB = 0; // Neg input is VR-
    AD1CHSbits.CH0SB = 0; // Pos input is AN0
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0; // Same as CH0SB
    
    // Timer 3 setup
    T3CON = 0;
    TMR3 = 0;
    T3CONbits.TON = 0;
    T3CONbits.TCKPS = 0b10; // 64 PRE
    PR3 = 15624; // Value for 0.0625s or every 1/16 of a second
    T3CONbits.TON = 1; // Turn on
    
    // Timer 2 setup
    T2CON = 0;
    TMR2 = 0;
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 0b10; // 64 PRE
    PR2 = 24999; // Value for ~0.1s or every 100ms
    T2CONbits.TON = 1;
    
    _T2IP = 7; // Highest priority
    
    // Enable AD1 interrupt 
    IEC0bits.AD1IE = 1; // Interrupt after ADC complete
    IFS0bits.AD1IF = 0;
    
    // Enable T2 interrupt
    IEC0bits.T2IE = 1; // Timer2 interrupt on ADC complete
    IFS0bits.T2IF = 0;
}


void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0;
    unsigned long int value = ADC1BUF0;
    putVal(value);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    unsigned long avg;
    _T2IF = 0; 
    TMR2 = 0;
    char adStr[20];
    lcd_setCursor(0,0);
    avg = getAvg();
    sprintf(adStr, "%6.4f V", (3.3/1024)*avg);
    lcd_printStr(adStr);
}

int main(void) {
    setup();
    while(1);
    return 1;
}