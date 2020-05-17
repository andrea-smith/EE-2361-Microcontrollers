#include "xc.h"
#include "ocall018_button.h"
#include "ocall018_servo.h"

//void initPushButton();
//void put(unsigned long int n);
//unsigned long int get(void);
//void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
//void __attribute__((__interrupt__, __auto_psv__)) _IC1Interrupt(void);

void initPushButton() {
    //Configure Timer 2 (500ns / count, 25ms max).
    // note that resolution = 500ns = 8 x 62.5ns, max period = 25ms = Tcy * 8 * 50,000
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 0b11;
    T2CONbits.TCS = 0b0;
    T2CONbits.TGATE = 0b0;
    TMR2 = 0;
    // Initialize to zero (also best practice)
    PR2 = 0xf424; // Set period to be larger than max external sig duration
    T2CONbits.TON = 1; // Start 16-bit Timer2
    
    // Initialize the Input Capture Module
    IC1CONbits.ICTMR = 1; // Select Timer2 as the IC1 Time base
    IC1CONbits.ICI = 0b00; // Interrupt on every capture event
    IC1CONbits.ICM = 0b011; // Generate capture event on every Rising edge
    // Enable Capture Interrupt And Timer2
    
    IPC0bits.IC1IP = 1; // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1; // Enable IC1 interrupt
    
    IFS0bits.T2IF = 0; // Clear IC1 Interrupt Status Flag
    IEC0bits.T2IE = 1; // Enable IC1 interrupt
}

volatile unsigned long int buffer[4];
unsigned char front = 0;
unsigned char back = 3;


void put(unsigned long int n) {
    buffer[front++] = n;
    front &= 3;
}

unsigned long int get(void) {
    unsigned long int i;
    i = buffer[back++];
    back &= 3;
    return i;
}
volatile unsigned long int seconds = 0;

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;
    seconds++;
}

unsigned long int lastTime = 0;
unsigned long int oldSeconds = 0;

void __attribute__((__interrupt__, __auto_psv__)) _IC1Interrupt(void) {
    unsigned long int val;
    unsigned long int period;
    unsigned long int pulseWidth;
    _IC1IF = 0;
    //T2CONbits.TON = 0; //Stops 16-bit Timer y
    val = IC1BUF;
    period = (val + (seconds * 62500)) - (lastTime + (oldSeconds * 62500));
    lastTime = val;
    pulseWidth = (period * 62.5 * 256 * 0.000001);
    if(pulseWidth > 2 && pulseWidth < 1000) {
	put(pulseWidth);
    }
    oldSeconds = seconds;
    //seconds = 0;
    //T2CONbits.TON = 1;
}