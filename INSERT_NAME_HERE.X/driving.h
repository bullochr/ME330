
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
#include "stdlib.h"
#include "state.h"

//return number of steps for a wheel to take, given the desired distance traveled
int getNumSteps(int dist_mm){
    return (int)((abs(dist_mm) * (float)(STEPS_PER_REV / (PI * WHEEL_DIAMETER_MM))));
}

void resetWheelsState(void){
    flags.right_wheel_reached_target = 0; //reset flag
    state.right_wheel_num_completed_steps = 0; //reset number of completed steps
    state.right_wheel_target_steps = 0; //reset target steps

    flags.left_wheel_reached_target = 0; //reset flag
    state.left_wheel_num_completed_steps = 0; //reset number of completed steps
    state.left_wheel_target_steps = 0; //reset target steps
}

void RotateLeftWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
//        state.left_wheel_direction = 1;
        _LATB8 = FORWARD; //forward
    }else{
//        state.left_wheel_direction = -1;
        _LATB8 = BACKWARD; //backward
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
//        state.right_wheel_direction = 1;
        _LATB9 = FORWARD; //forward
    }else{
//        state.right_wheel_direction = -1;
        _LATB9 = BACKWARD; //backward
    }
    state.right_wheel_target_steps = abs(num_steps);
    OC2CON1bits.OCM = 0b110; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
}

void driveForwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    _LATB8 = FORWARD; //Drive right wheel forward
    _LATB9 = FORWARD; //Drive left wheel forward
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
    left_wheel_PWM_state = PWM_ON;
    right_wheel_PWM_state = PWM_ON;
}

void driveBackwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    _LATB8 = BACKWARD; //Drive right wheel backward
    _LATB9 = BACKWARD; //Drive right wheel backward
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
    left_wheel_PWM_state = PWM_ON;
    right_wheel_PWM_state = PWM_ON;
}

//make a 90 degree turn. Pass a 1 to turn left, or a 0 to turn right
void turn90Degrees(int turn_left){
    int dist_mm = (int)(0.25*PI*ROBOT_WIDTH_MM) + 20; //calculates num_steps to travel 1/4 circumference of a circle with diam = ROBOT_WIDTH, with some buffer to account for skipping steps
    if(turn_left){ //rotate wheels equally in opposing directions
        RotateLeftWheelDistance(-dist_mm);
        RotateRightWheelDistance(dist_mm); 
    }else{
        RotateLeftWheelDistance(dist_mm);
        RotateRightWheelDistance(-dist_mm); 
    }
}

void initializeLineFollowing(void){
    _LATB8 = FORWARD; //Drive right wheel forward
    _LATB9 = FORWARD; //Drive left wheel forward
    
    //clear target step values
    state.right_wheel_target_steps = 0;
    state.left_wheel_target_steps = 0;
    
    //enable PWM on both wheels
    left_wheel_PWM_state = PWM_ON;
    right_wheel_PWM_state = PWM_ON;
}

char allQrdsOnLine(void){
    return (left_qrd_value > WHITE_THRESHOLD && center_qrd_value > WHITE_THRESHOLD && right_qrd_value > WHITE_THRESHOLD);
}

int getCorrectionFactor(void){
    //calculate signed error: a value between ... that describes how far from the line we are 
    int e_p = ((int)right_qrd_value - (int)left_qrd_value);
    
    static int last_e_p = 0;
    static int e_i = 0;
    
    //add error to accumulated error
    e_i = e_i + e_p;
    
    //find rate of change of error since last loop
    int e_d = e_p - last_e_p;
    
    last_e_p = e_p;
    
    int correction = e_p * KP;
    if (correction > MAX_CORRECTION_VALUE){
        correction = MAX_CORRECTION_VALUE;
        led_1_state = 1;
    }else{
        led_1_state = 0;
    }
    if (correction < (-1 * MAX_CORRECTION_VALUE)){
        correction = -1 * MAX_CORRECTION_VALUE;
        led_2_state = 1;
    }else{
        led_2_state = 0;
    }
    return correction;
}

#endif	/* driving */

