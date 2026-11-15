
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
    state.right_wheel_target_period = BASE_WHEEL_PERIOD;
    
    flags.left_wheel_reached_target = 0; //reset flag
    state.left_wheel_num_completed_steps = 0; //reset number of completed steps
    state.left_wheel_target_steps = 0; //reset target steps
    state.left_wheel_target_period = BASE_WHEEL_PERIOD;
}

void RotateLeftWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        left_wheel_dir = FORWARD; //forward
    }else{
        left_wheel_dir = BACKWARD; //backward
    }
    left_wheel_dir = FORWARD;
    state.left_wheel_target_steps = abs(num_steps);
    left_wheel_PWM_state = PWM_ON; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
}

//takes a distance in mm, turns right wheel on in the direction specified by the
//whether dist_mm is positive or negative, and sets the target steps, so that the
//interrupt function will know when to stop
void RotateRightWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        right_wheel_dir = FORWARD; //forward
    }else{
        right_wheel_dir = BACKWARD; //backward
    }
    state.right_wheel_target_steps = abs(num_steps);
    right_wheel_PWM_state = PWM_ON; // ENABLE PWM on pin 14 (edge-aligned PWM pulses)
}

void driveForwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    left_wheel_dir = FORWARD; 
    right_wheel_dir = FORWARD;
    
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
    return (left_qrd_value < WHITE_THRESHOLD && center_qrd_value < CENTER_QRD_WHITE_THRESHOLD && right_qrd_value < WHITE_THRESHOLD);
}

char allQRDsBlack(void){
    return (left_qrd_value > BLACK_THRESHOLD && center_qrd_value > CENTER_QRD_WHITE_THRESHOLD && right_qrd_value > BLACK_THRESHOLD);
    return (center_qrd_value > BLACK_THRESHOLD);
}

char bothWheelsFinished(){
    if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
        resetWheelsState();
        return 1;
    } else{
        return 0;
    }
}

int getCorrectionFactor(void){
    //calculate signed error: a value between ... that describes how far from the line we are 
    float e_p = ((int)right_qrd_value - (int)left_qrd_value);
    
    static int last_e_p = 0;
    static float e_i = 0;
    float elapsed_time = TMR1; //value in TMR1 is the elapsed time since last measurement
    TMR1 = 0; //reset TMR1, which measures the time between measurements
    
    //add error to accumulated error
    e_i = e_i + (e_p * elapsed_time);
    if (e_i > MAX_INTEGRAL_VALUE){e_i = MAX_INTEGRAL_VALUE;} 
    else if (e_i < -MAX_INTEGRAL_VALUE){e_i = -MAX_INTEGRAL_VALUE;}
    
    //find rate of change of error since last loop
    float e_d = (e_p - last_e_p) / elapsed_time;
    
    last_e_p = e_p;
    
    int correction = (e_p * KP) + (e_i * KI) + (e_d * KD);
    return correction;
}

void followLine(){
    led_1_state = 0;
    int correction = getCorrectionFactor();
    int slow_down_factor = correction * SLOW_DOWN_COEFFICIENT;
    
    static int left_period = 0;
    static int right_period = 0;
    left_period = BASE_WHEEL_PERIOD - correction + slow_down_factor;
    right_period = BASE_WHEEL_PERIOD + correction + slow_down_factor;

    //force period to be greater than min period
    //turn on led while period = min_period (max turning speed)
    if (left_period < MIN_PERIOD){
        left_period = MIN_PERIOD;
        led_1_state = 1;
    }
    if (right_period < MIN_PERIOD){
        right_period = MIN_PERIOD;
        led_1_state = 1;
    }

    //apply correction factor to wheel speeds
    state.left_wheel_target_period = left_period;
    state.right_wheel_target_period = right_period;
}

void initializeCanyonNavigation(void){
    driveForwardDistance(610);
}

#endif	/* driving */

