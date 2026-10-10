#include "xc.h"
#include "global_variables.h" //includes all global vaariables defined in the header file with this name
#include "ISR_functions.h" //includes all interrupt service routine function definitions in the header file with this name
#include "driving.h" //all functions related to motion of the robot
#include "pin_config.h" //configure pin settings
#include "state.h" //defines state struct and flag struct and functions to initialize them

//Oscillator setup
#pragma config FNOSC = FRC //F_osc = 8MHz, F_cy = 4MHz 

void stateMachine(){
    switch(state.state_num){
        
        case 0: //line following
            //if left sensor is low(black), turn right
            //if right sensor is high(white), turn left
            break;
            
        case 1: //basic motion
            driveForwardDistance(500); //forward 500mm
            turn90Degrees(0); //right turn
            driveForwardDistance(500); //forward 500mm
            turn90Degrees(0); //right turn
            turn90Degrees(0); //right turn
            driveForwardDistance(500); //forward 500mm
            state.state_num = 0; //reset state to line following
            break;
            
        case 10: 
            break;
        
        case 20:
            break;
            
        case 30:
            break;
        
        default:
            break;
            
    }
}

//react to custom flags and make decisions about the state
void updateState(){
    if(flags.right_wheel_reached_target){
        flags.right_wheel_reached_target = 0; //reset flag
        state.right_wheel_num_completed_steps = 0; //reset number of completed steps
        state.right_wheel_target_steps = 0; //reset target steps
        OC2CON1bits.OCM = 0b000; // DISABLE PWM for right wheel
    }
    
    if(flags.left_wheel_reached_target){
        flags.left_wheel_reached_target = 0; //reset flag
        state.left_wheel_num_completed_steps = 0; //reset number of completed steps
        state.left_wheel_target_steps = 0; //reset target steps
        OC1CON1bits.OCM = 0b000; // DISABLE PWM for left wheel
    }
    
    //get sensor data
    //make state change decisions
}

int main(void) {
    //Clear registers
    OC1CON1 = 0; //Pin 14
    OC1CON2 = 0;
    OC2CON1 = 0; //Pin 4
    OC2CON2 = 0;
    OC3CON1 = 0; //Pin 5
    OC3CON2 = 0;
       
    //configure pins based on a function in pin_config.h
    configure_pins();
    
    //initialize a struct to hold the states of each custom flag
    flags = initializeFlags();
    
    //initialize a struct to hold the variables that define the state of the robot
    state = initializeState();
    
    state.state_num = 1; //set initial state to basic motion for milestone 5
    
    while(1){
        updateState();
        stateMachine();
    }
    
    return 0;
}
