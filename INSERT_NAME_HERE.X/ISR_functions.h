/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

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

//left wheel completed one step, called on every PWM rising edge
//if wheel has reached it's target number of steps, turn off pwm and reset state
void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void){
    _OC1IF = 0;
    state_var.left_wheel_completed_steps++;
    if(state_var.left_wheel_completed_steps >= state_var.left_wheel_target_steps){ //if left wheel has reached target number of steps
        state_var.left_wheel_completed_steps = 0; //reset number of completed steps
        state_var.left_wheel_target_steps = 0; //reset target steps
        state_var.left_wheel_reached_target = 1; //set flag that left wheel has finished the commanded number of steps
        OC1CON1bits.OCM = 0b000; // DISABLE PWM on pin 14
    }
}

//right wheel completed one step, called on every PWM rising edge
//if wheel has reached it's target number of steps, turn off pwm and reset state
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void){
    _OC2IF = 0;
    state_var.right_wheel_completed_steps++;
    if(state_var.right_wheel_completed_steps >= state_var.right_wheel_target_steps){ //if left wheel has reached target number of steps
        state_var.right_wheel_completed_steps = 0; //reset number of completed steps
        state_var.right_wheel_target_steps = 0; //reset target steps
        state_var.right_wheel_reached_target = 1; //set flag that left wheel has finished the commanded number of steps
        OC2CON1bits.OCM = 0b000; // DISABLE PWM on pin 14
    }
}


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

