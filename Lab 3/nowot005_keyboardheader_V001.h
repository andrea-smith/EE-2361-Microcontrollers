/* 
 * File:   nowot005_keyboardheader_V001.h
 * Author: Owner
 *
 * Created on October 4, 2019, 9:10 AM
 */

#ifndef NOWOT005_KEYBOARDHEADER_V001_H
#define	NOWOT005_KEYBOARDHEADER_V001_H

#ifdef	__cplusplus
extern "C" {
#endif

void initKeyPad(void);
int tryColumns(int row);
void delay(unsigned int milli);
unsigned int readKeyPadRaw(void);


#ifdef	__cplusplus
}
#endif

#endif	/* NOWOT005_KEYBOARDHEADER_V001_H */

