
#ifndef DINO_LIB_H
#define	DINO_LIB_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif 

    void setup_dino(void);
    void action(int move);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

