/*
 * Sitronix I2C LCD Library
 * Library to display a scrolling string on the Sitronix I2C LCD using the PIC24FJ64GA002 microcontroller.
 * 
 * File:   smit9523_Lab5_main.c
 * Author: Andrea Smith
 *
 * Created on April 10, 2020, 4:24 PM
 */


#include "xc.h"
#include "string.h"
#include "stdint.h"
#include "smit9523_libH.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
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

void lcd_cmd(char command);
void lcd_init(void);
void delay(int milliseconds);

void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff; // All i/o pins digital
    PORTAbits.RA0 = 0; // For the heartbeat LED
    I2C2CONbits.I2CEN = 0; // Good practice to disable before changing BRG
    I2C2BRG = 0x9D; // Value for 16 MHz
    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0; // Clear interrupt, just in case
    
    lcd_init();
}

// Function: void
// Calls ASM 1 millisecond delay (see smit9523_libASM.s) the arg, milliseconds, amount of times
void delay(int milliseconds) {
    int i = 0;
    for (i = 0; i < milliseconds; i++) {
        delay_1ms();
    }
}

// Function: lcd_cmd(char command)
// Writes out command in I2C bus. Contains start bit, address + R/nW, control, and command bytes, and stop bit.
void lcd_cmd(char command) {
    I2C2CONbits.SEN = 1;    // Begin start sequence 
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;   // MI2C2IF needs to clear software before every send
    I2C2TRN = 0b01111100;   // Slave address and R/nW bit
    while(!IFS3bits.MI2C2IF); 
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000;   // Control bytes
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command;      // 8-bits consisting of the data byte
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN); // PEN will clear when Stop bit is complete
}

// Function : lcd_init
// Implements the complete sequence of lcd_cmd() writes and delays to initialize the LCD
void lcd_init(void) {

    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    delay(2);

}

// Function: lcd_setCursor
// Sets cursor to row y, column x
void lcd_setCursor(char x, char y) {
    char address = (x + 0x40) * y;
    char packet = address + (1 << 7);
    lcd_cmd(packet);
}

// Function: lcd_printChar
// Verifies LCD operation. Contains start bit, address + R/nW, control, and command bytes, and stop bit.
// Capable of printing a single character to the LCD.
void lcd_printChar(char myChar) {
    I2C2CONbits.SEN = 1; // Start
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // Slave address bits (7 bits) and R/nW bit (1 bit)
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000; // Control byte and Rs = 1
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar; // Data byte
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1; // Stop
    while(I2C2CONbits.PEN);
}

// Function: lcd_printStr
// Takes arbitrary length string and sends it to the LCD. Contains multiple command/data byte pairs.
void lcd_printStr(const char s[]) {
    I2C2CONbits.SEN = 1; // Start
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // Slave address bits (7) and R/nW bit (1)
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    int i = 0;
    int length = strlen(s);
    
    for (i = 0; i < (length-1); i++) {
        I2C2TRN = 0b11000000; // Control byte, Co = 1 & Rs = 1, first of several control/data byte pairs
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        I2C2TRN = s[i];
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;   
    }
    
    I2C2TRN = 0b01000000; // Final byte, Co = 0 & Rs = 1;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1; // Stop
    while(I2C2CONbits.PEN);
}

// Function: left
// Scrolls text to from left to right. See Cursor or Display Shift on pg 22 of I2C ST7032 Sitronix LCD documentation.
void left(void){
    lcd_cmd(0b00011000);
}

int main(void) {
    setup();
    lcd_setCursor(0,0); 
    //lcd_printChar('C'); 
    lcd_printStr("What is the meaning of life?    42.");
    
    
    while(1) {
        delay(150);
        left();
    }
}
