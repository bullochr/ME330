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
#ifndef DRIVING
#define	DRIVING

#include <xc.h> // include processor files - each processor file is guarded.  
#include "global_variables.h"
#include "state.h"

//return number of steps for a wheel to take, given the desired distance traveled
int getNumSteps(int dist_mm){
    return (int)(dist_mm * (STEPS_PER_REV / (PI * WHEEL_DIAMETER_MM)));
}

void RotateLeftWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        state.left_wheel_direction = 1;
        _LATB9 = 1; //TODO assign actual direction
    }else{
        state.left_wheel_direction = -1;
        _LATB9 = 0; //TODO assign actual direction
    }
    state.left_wheel_target_steps = num_steps;
    OC1CON1bits.OCM = 0b110; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
}

//takes a distance in mm, turns right wheel on in the direction specified by the
//whether dist_mm is positive or negative, and sets the target steps, so that the
//interrupt function will know when to stop
void RotateRightWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        state.right_wheel_direction = 1;
        _LATB8 = 0; //TODO assign actual direction
    }else{
        state.right_wheel_direction = -1;
        _LATB8 = 1; //TODO assign actual direction
    }
    state.right_wheel_target_steps = num_steps;
    OC2CON1bits.OCM = 0b110; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
}

void driveForwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    state.right_wheel_direction = 1;
    state.left_wheel_direction = -1;
    _LATB8 = 0; //TODO assign actual direction
    _LATB9 = 0; //TODO assign actual direction
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
    OC1CON1bits.OCM = 0b110; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
    OC2CON1bits.OCM = 0b110; // ENABLE PWM on pin 4 (edge-aligned PWM pulses)
}

void driveBackwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    state.right_wheel_direction = -1;
    state.left_wheel_direction = 1;
    _LATB8 = 1; //TODO assign actual direction
    _LATB9 = 1; //TODO assign actual direction
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
    OC1CON1bits.OCM = 0b110; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
    OC2CON1bits.OCM = 0b110; // ENABLE PWM on pin 4 (edge-aligned PWM pulses)
}

//make a 90 degree turn. Pass a 1 to turn left, or a 0 to turn right
void turn90Degrees(int turn_left){
    int dist_mm = (int)(0.25*PI*ROBOT_WIDTH_MM); //calculates num_steps to travel 1/4 circumference of a circle with diam = ROBOT_WIDTH
    if(turn_left){ //rotate wheels equally in opposing directions
        RotateLeftWheelDistance(-dist_mm);
        RotateRightWheelDistance(dist_mm); 
    }else{
        RotateLeftWheelDistance(dist_mm);
        RotateRightWheelDistance(-dist_mm); 
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

