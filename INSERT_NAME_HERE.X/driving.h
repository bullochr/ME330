
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

void setLeftWheelVelocity(float vel){
    if (vel == 0){
        left_wheel_PWM_state = PWM_OFF;
        state.left_wheel_velocity = 0; //TODO: gradual braking to stop
    } else{
        left_wheel_PWM_state = PWM_ON;
        if(vel > MAX_WHEEL_VELOCITY){
            vel = MAX_WHEEL_VELOCITY;
        }
    }
    state.left_wheel_target_velocity = vel;
}

void setRightWheelVelocity(float vel){
    if (vel == 0){
        right_wheel_PWM_state = PWM_OFF;
        state.right_wheel_velocity = 0; //TODO: gradual braking to stop
    } else{
        right_wheel_PWM_state = PWM_ON;
        if(vel > MAX_WHEEL_VELOCITY){
            vel = MAX_WHEEL_VELOCITY;
        }
    }
    state.right_wheel_target_velocity = vel;
}


void resetWheelsState(void){
    flags.right_wheel_reached_target = 0; //reset flag
    state.right_wheel_num_completed_steps = 0; //reset number of completed steps
    state.right_wheel_target_steps = 0; //reset target steps
    state.right_wheel_target_velocity = BASE_WHEEL_VELOCITY;
    
    flags.left_wheel_reached_target = 0; //reset flag
    state.left_wheel_num_completed_steps = 0; //reset number of completed steps
    state.left_wheel_target_steps = 0; //reset target steps
    state.left_wheel_target_velocity = BASE_WHEEL_VELOCITY;
}

void RotateLeftWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
    }else{
        setLeftWheelVelocity(-BASE_WHEEL_VELOCITY);
    }
    state.left_wheel_target_steps = abs(num_steps);
}

//takes a distance in mm, turns right wheel on in the direction specified by the
//whether dist_mm is positive or negative, and sets the target steps, so that the
//interrupt function will know when to stop
void RotateRightWheelDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm); //calculate steps required to go desired distance
    if (dist_mm >= 0) {
        setRightWheelVelocity(BASE_WHEEL_VELOCITY);
    }else{
        setRightWheelVelocity(-BASE_WHEEL_VELOCITY);
    }
    state.right_wheel_target_steps = abs(num_steps);
}

void driveForwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    setRightWheelVelocity(BASE_WHEEL_VELOCITY);
    setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
}

void driveBackwardDistance(int dist_mm){
    int num_steps = getNumSteps(dist_mm);
    setRightWheelVelocity(-BASE_WHEEL_VELOCITY);
    setLeftWheelVelocity(-BASE_WHEEL_VELOCITY);
    
    state.right_wheel_target_steps = num_steps;
    state.left_wheel_target_steps = num_steps;
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
    //clear target step values
    state.right_wheel_target_steps = 0;
    state.left_wheel_target_steps = 0;
    
    setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
    setRightWheelVelocity(BASE_WHEEL_VELOCITY);
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
    int correction = getCorrectionFactor();
//    int slow_down_factor = correction * SLOW_DOWN_COEFFICIENT;
    int slow_down_factor = 0;
    
    int left_velocity = BASE_WHEEL_VELOCITY + correction - slow_down_factor;
    int right_velocity = BASE_WHEEL_VELOCITY - correction - slow_down_factor;

    //apply correction factor to wheel speeds
    setRightWheelVelocity(right_velocity);
    setLeftWheelVelocity(left_velocity);
}

void initializeCanyonNavigation(void){
    driveForwardDistance(610);
}

//takes wheel velocity in steps/second, returns PWM period
int velocityToPeriod(float vel){
    int period;
    if(vel < 70.0){ //this will cause integer to overflow
        period = 65000; //slowest value
    }else{
        period = (CLOCK_FREQUENCY / vel);
    }
    return period;
}

//float periodToVelocity(int period){
//    return CLOCK_FREQUENCY / period;
//}

void updateWheelVelocity(){
    //LEFT WHEEL
    static char last_dir_left = 2;
    
    //check if velocity is within the step size (RAMP_UP_SPEED) of the target
    if (state.left_wheel_velocity != state.left_wheel_target_velocity){
        if(abs(state.left_wheel_velocity - state.left_wheel_target_velocity) > RAMP_UP_SPEED){
            if(state.left_wheel_velocity < state.left_wheel_target_velocity){
                state.left_wheel_velocity += RAMP_UP_SPEED;
            } else{
                state.left_wheel_velocity -= RAMP_UP_SPEED;
            }
        } else{
            state.left_wheel_velocity = state.left_wheel_target_velocity;
        }
        
        
        left_wheel_PWM_period = velocityToPeriod(abs(state.left_wheel_velocity));
        if (state.left_wheel_velocity > 0){
            left_wheel_dir = 1;
        } else{
            left_wheel_dir = 0;
        }

        //detect when left wheel changes direction (velocity changes sign)
        if (last_dir_left != left_wheel_dir){
            if(left_wheel_dir == 1){
                left_wheel_dir = FORWARD;
            } else{
                left_wheel_dir = BACKWARD;
            }
        }
        last_dir_left = left_wheel_dir;
    }
    
    
    //RIGHT WHEEL
    static char last_dir_right = 2;
    
    //check if velocity is within the step size (RAMP_UP_SPEED) of the target
    if (state.right_wheel_velocity != state.right_wheel_target_velocity){
        
        if(abs(state.right_wheel_velocity - state.right_wheel_target_velocity) > RAMP_UP_SPEED){
            if(state.right_wheel_velocity < state.right_wheel_target_velocity){
                state.right_wheel_velocity += RAMP_UP_SPEED;
            } else{
                state.right_wheel_velocity -= RAMP_UP_SPEED;
            }
        } else{
            state.right_wheel_velocity = state.right_wheel_target_velocity;
        }
    
        right_wheel_PWM_period = velocityToPeriod(abs(state.right_wheel_velocity));
        if (state.right_wheel_velocity > 0){
            right_wheel_dir = 1;
        } else{
            right_wheel_dir = 0;
        }

        //detect when wheel changes direction (velocity changes sign)
        if (last_dir_right != right_wheel_dir){
            if(right_wheel_dir == 1){
                right_wheel_dir = FORWARD;
            } else{
                right_wheel_dir = BACKWARD;
            }
        }
        last_dir_right = right_wheel_dir;
    }
}

#endif	/* driving */

