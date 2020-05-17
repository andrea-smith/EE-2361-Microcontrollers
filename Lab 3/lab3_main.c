/*
 * File:   lab3_main.c
 * Author: Owner
 *
 * Created on October 4, 2019, 7:05 AM
 */


#include "xc.h"
#include "nowot005_displayheader_V001.h"
#include "nowot005_keyboardheader_V001.h"

//CW1: FLASH CONFIGURATION WORD 1

#pragma config ICS = PGx1       // Comm Channel Select
#pragma config FWDTEN = OFF     // Watchdog Timer Disable
#pragma config GWRP = OFF       // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF        // General Code Segment COde Protect (Code protection is disabled)
#pragma config JTAGEN = OFF     // JTAG port enable (JTAG port is disabled)

//CW2: FLASH CONFIGURATION WORD 2

#pragma config I2C1SEL = PRI    // I2C1 Pin Location Select
#pragma config IOL1WAY = OFF    // IOLOCK Protection
#pragma config OSCIOFNC = ON    // Primary Oscillator I/O Function
#pragma config FCKSM = CSECME   // Clock Switching and Monitor
#pragma config FNOSC = FRCPLL   // Oscillator Select

#define PERIOD 5

void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;
    init7seg();
    initKeyPad();
}

char map_func[4][4] = {
    {'E','7','4','1'},
    {'0','8','5','2'},
    {'F','9','6','3'},
    {'D','C','B','A'}
};


int main(void) {
    setup();
    
    char L_Char = '\0';
    char R_Char = '\0';
    int prevKey = -1;
    while(1){
        int newKey = readKeyPadRaw();
        if (newKey >= 0){
            //if(newKey != prevKey){
                R_Char = map_func[(newKey/10)%10][newKey%10];
                if (prevKey >= 0){
                    L_Char = map_func[(prevKey/10)%10][prevKey%10];
                }
                prevKey = newKey;
         //   }
        }
        
        if (R_Char != '\0'){
            showChar7seg(R_Char, msb);
            delay(10); //10ms
        }
        if (L_Char != '\0'){
            showChar7seg(L_Char, lsb);
            delay(10); //10ms
        }
    }
}
