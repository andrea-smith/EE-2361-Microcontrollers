/* 
 * File:   smit9523_libH.h
 * Author: Andrea Smith
 * Comments: 
 * Revision history: N/A
 */

#ifndef SMIT9523_LIBH_H
#define	SMIT9523_LIBH_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus

#endif 

    void setup(void);
    void delay(int milliseconds);
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char x, char y);
    void lcd_printStr(const char s[]);
    void left(void);
    
    // ASM function
    void delay_1ms(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif
