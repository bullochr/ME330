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
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                turn90Degrees(1); //right turn
                state.state_num++;
            }
            break;
        case 2:
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(500); //right turn
                state.state_num++;
            }
            break;
        case 3:
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                turn90Degrees(0); //right turn
                state.state_num++;
            }
            break;
        case 4:
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                turn90Degrees(0); //right turn
                state.state_num++;
            }
            break;
        case 5:
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(500); //right turn
                state.state_num++;
            }
            break;
        case 6:
            if(flags.right_wheel_reached_target && flags.left_wheel_reached_target){
                resetWheelsState();
                state.state_num = 0;
            }
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
    
    driveForwardDistance(500); //forward 500mm
    
    while(1){
        updateState();
        stateMachine();
    }
    
    return 0;
}
