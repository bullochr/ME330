
#ifndef WAIT
#define	WAIT

#include <xc.h>

#include "state.h" // include processor files - each processor file is guarded.  

void waitMsBlocking(int ms){
    ms = ms / 4; //MYstEry COeFFiCienT
    T4CON = 0; //clears timer 5 configuration register
    TMR4 = 0;
    T4CONbits.TCKPS = 0b11; //sets a prescaler value of 256
    T4CONbits.TCS = 0; //sets the clock to the internal oscillator
    T4CONbits.TON = 1; //turn on timer 5
    PR4 = (int)(ms * 15.625);
    
    
    _T4IP = 4; //priority
    _T4IE = 1; //enable interrupt
    _T4IF = 0; //reset flag
    while(!flags.finished_waiting){
    //do nothing
    }
    flags.finished_waiting = 0;
    
    _T4IE = 0; //enable interrupt
    T4CONbits.TON = 0; //turn on timer 5
}

#endif	/* XC_HEADER_TEMPLATE_H */

