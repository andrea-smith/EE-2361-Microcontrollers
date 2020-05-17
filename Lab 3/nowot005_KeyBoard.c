#include <p24FJ64GA002.h>

#include "xc.h"
#include "nowot005_keyboardheader_V001.h"

void initKeyPad(void);
unsigned int readKeyPadRaw(void);

void initKeyPad(void){
    TRISA |= 0x000F;//ra3-ra0 input
    TRISB &= 0x0FFF;//rb15-rb12 output
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    CNPU2bits.CN29PUE = 1;
    CNPU2bits.CN30PUE = 1;
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

int tryColumns(int row){
    unsigned int bitmask = 0xFFF7;
    int jj;
    
    for(jj = 0; jj < 4; jj++){
        if ((PORTA | bitmask) < 0xFFFF){
            return jj;
        }
        bitmask = (bitmask >> 1);
        bitmask += 0x8000;
    }
    return -1;
}
/*try columns starts with the bitmask being 
 * 0b1111 1111 1111 0111 for jj = 0
 * 0b1111 1111 1111 1011 for jj = 1
 * 0b1111 1111 1111 1101 for jj = 2
 * 0b1111 1111 1111 1110 for jj = 3
 * 
 * this allows for us to check rb15-rb12 for the output from the keypad*/

unsigned int readKeyPadRaw(void) {
    unsigned int bitmask = 0x7FFF;
    int i;
    int result;
    static int previous = -1;
    
    //bottom to top
    for(i = 0; i < 4; i++) {
        LATB |= 0xF000;
        LATB &= bitmask;
        result = tryColumns(i);
        
        if (result != previous) {
            result = tryColumns(i);
            if (result >= 0) {
                LATB &= 0x0FFF;
                return (result*10) + i; // the reason we are doing this is for indexing reasons
            }
        }
        previous = result;
        bitmask = bitmask >> 1;
        bitmask += 0x8000;
    }
    return -1;
}








//unsigned int readKeyPadRaw(void){
//    int columnA, columnB, columnC, columnD;
//    //BOTTOM ROW
//    LATBbits.LATB15 = 0; //Row 4
//    LATBbits.LATB14 = 1; //Row 3
//    LATBbits.LATB13 = 1; //Row 2
//    LATBbits.LATB12 = 1; //Row 1
//    delay();
//    columnA = PORTAbits.RA0;
//    columnB = PORTAbits.RA1;
//    columnC = PORTAbits.RA2;
//    columnD = PORTAbits.RA3;
//    
//    if(!columnA){
//        return 0xD;
//    } else if (!columnB){
//        return 0xF;
//    } else if (!columnC){
//        return 0;
//    } else if (!columnD){
//        return 0xF;
//    }
//    //ROW 3
//    LATBbits.LATB15 = 1; //Row 4
//    LATBbits.LATB14 = 0; //Row 3
//    LATBbits.LATB13 = 1; //Row 2
//    LATBbits.LATB12 = 1; //Row 1
//    delay();
//    columnA = PORTAbits.RA0;
//    columnB = PORTAbits.RA1;
//    columnC = PORTAbits.RA2;
//    columnD = PORTAbits.RA3;
//    
//    if(!columnA){
//        return 0xC;
//    } else if (!columnB){
//        return 0x9;
//    } else if (!columnC){
//        return 0x8;
//    } else if (!columnD){
//        return 0x7;
//    }
//    //ROW 2
//    LATBbits.LATB15 = 1; //Row 4
//    LATBbits.LATB14 = 1; //Row 3
//    LATBbits.LATB13 = 0; //Row 2
//    LATBbits.LATB12 = 1; //Row 1
//    delay();
//    columnA = PORTAbits.RA0;
//    columnB = PORTAbits.RA1;
//    columnC = PORTAbits.RA2;
//    columnD = PORTAbits.RA3;
//    
//    if(!columnA){
//        return 0xB;
//    } else if (!columnB){
//        return 0x6;
//    } else if (!columnC){
//        return 0x5;
//    } else if (!columnD){
//        return 0x4;
//    }
//    //TOP ROW
//    LATBbits.LATB15 = 1; //Row 4
//    LATBbits.LATB14 = 1; //Row 3
//    LATBbits.LATB13 = 1; //Row 2
//    LATBbits.LATB12 = 0; //Row 1
//    delay();
//    columnA = PORTAbits.RA0;
//    columnB = PORTAbits.RA1;
//    columnC = PORTAbits.RA2;
//    columnD = PORTAbits.RA3;
//    
//    if(!columnA){
//        return 0xA;
//    } else if (!columnB){
//        return 0x3;
//    } else if (!columnC){
//        return 0x2;
//    } else if (!columnD){
//        return 0x1;
//    }
//    
//}