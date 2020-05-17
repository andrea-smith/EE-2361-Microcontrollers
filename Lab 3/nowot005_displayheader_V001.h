/* 
 * File:   Display Header
 * Author: Jason Nowotny
 * Comments:
 * Revision history: 10 - 4 - 2019
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef nowot005_displayheader_V001_H
#define	nowot005_displayheader_V001_H

#include <xc.h> // include processor files - each processor file is guarded.  



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define CLEAR_DIGS_AND_SEGS_BIT_MASK 0b0000000111111100// to be anded
    
enum DIGIT{
    msb = 0b0000100000000000,
    lsb = 0b0000010000000000
};

void init7seg(void);
void showChar7seg(char myChar, enum DIGIT myDigit);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

