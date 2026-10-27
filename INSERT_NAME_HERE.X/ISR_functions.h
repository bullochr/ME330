
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_ISR_FUNCTIONS
#define	XC_ISR_FUNCTIONS

#include <xc.h> // include processor files - each processor file is guarded.  
#include "state.h"
#include "global_variables.h"

//left wheel completed one step, called on every PWM rising edge
//if wheel has reached it's target number of steps, turn off pwm and reset state
void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void){
    _OC1IF = 0;
    if (state.left_wheel_target_steps != 0){ //only run the following code if there is a non-zero step limit set
        state.left_wheel_num_completed_steps++;
        if(state.left_wheel_num_completed_steps >= state.left_wheel_target_steps){ //if left wheel has reached target number of steps
            flags.left_wheel_reached_target = 1; //set flag that left wheel has finished the commanded number of steps
            left_wheel_PWM_state = PWM_OFF;
        }
    }
}

//right wheel completed one step, called on every PWM rising edge
//if wheel has reached it's target number of steps, turn off pwm and reset state
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void){
    _OC2IF = 0;
    if (state.right_wheel_target_steps != 0){
        state.right_wheel_num_completed_steps++;
        if(state.right_wheel_num_completed_steps >= state.right_wheel_target_steps){ //if right wheel has reached target number of steps
            flags.right_wheel_reached_target = 1; //set flag that right wheel has finished the commanded number of steps
            right_wheel_PWM_state = PWM_OFF;
        }
    }
}

//triggers when a set of analog to digital conversions finish
void __attribute__ ((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    _AD1IF = 0;
    flags.ad_conversions_finished = 1;
    
    //reset AD conversion. I don't know why this helps, but without it, the 
    //program freezes after running the interrupt once
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D 
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}

#endif	/* XC_HEADER_TEMPLATE_H */

