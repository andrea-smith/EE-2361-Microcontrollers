/* 
 * File:   Display
 * Author: Jason Nowotny
 * Comments:
 * Revision history: 10 - 4 - 2019
 */

#include "xc.h"
#include "nowot005_displayheader_V001.h"

void init7seg(void);
void showChar7seg(char myChar, enum DIGIT myDigit);

int pattern(char Char){
    switch(Char)
    {
        case '0'://        ABCDEFG.
            return 0b0000000000001100;
        case '1':
            return 0b0000001001111100;
        case '2':
            return 0b0000000010010100;
        case '3':
            return 0b0000000000110100;
        case '4':
            return 0b0000001001100100;
        case '5':
            return 0b0000000100100100;
        case '6':
            return 0b0000000100000100;
        case '7':
            return 0b0000000001111100;
        case '8':
            return 0b0000000000000100;
        case '9':
            return 0b0000000000100100;
        case 'A':
            return 0b0000000001000100;
        case 'B':
            return 0b0000001100000100; //b
        case 'C':
            return 0b0000000110001100;
        case 'D':
            return 0b0000001000010100; //d
        case 'E':
            return 0b0000000110000100;
        case 'F':
            return 0b0000000111000100;
        case 's':
            return 0b0000001001010100; //scorpion
        case 'a':
            return 0b0000001111111100; // all off
    }
}

void init7seg(void) {
    TRISB &= 0b1111000000000011; 
    LATB |=  0b0000111111111100; //turns off 7 seg LEDs
    LATB &=  0b1111111111111111; // turns off all displays
}

void showChar7seg(char myChar, enum DIGIT myDigit) {
    
    LATB &= CLEAR_DIGS_AND_SEGS_BIT_MASK;
    LATB = myDigit | pattern(myChar);
           
}