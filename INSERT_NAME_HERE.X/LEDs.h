

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LEDs
#define	LEDs

#include <xc.h> // include processor files - each processor file is guarded.  
#include "global_variables.h"

void blinkLED1(int num_blinks, int period = 10000){ //use Timer 2 to asynchronously blink LED
    T2CONbits.TON = 1;
    T2CONbits.TCS = 0;
    T2CONbits.TCKPS = 0;
    PR2 = period;
    TMR2 = 0;
    
    _T2IP = 4; //priority
    _T2IE = 1 //enable interrupt
    _T2IF = 0; //reset flag
    state.led_1_target_num_blinks = num_blinks;
    
    led_1_state = 1;
}

void blinkLED2(int num_blinks, int period = 10000){ //use Timer 2 to asynchronously blink LED
    T3CONbits.TON = 1;
    T3CONbits.TCS = 0;
    T3CONbits.TCKPS = 0;
    PR3 = period;
    TMR3 = 0;
    
    _T3IP = 4; //priority
    _T3IE = 1 //enable interrupt
    _T3IF = 0; //reset flag
    state.led_2_target_num_blinks = num_blinks;
    
    led_2_state = 1;
}

#endif	/* XC_HEADER_TEMPLATE_H */

