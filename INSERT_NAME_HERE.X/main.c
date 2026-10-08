#include "xc.h"
#include "global_variables.h" //includes all global vaariables defined in the header file with this name
#include "ISR_functions.h" //includes all interrupt service routine function definitions in the header file with this name
#include "driving.h" //all functions related to motion of the robot
#include "pin_config.h" //configure pin settings

//Oscillator setup
#pragma config FNOSC = FRC //F_osc = 8MHz, F_cy = 4MHz 

void stateMachine(){
    switch(state_var.state){
        
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
            state_var.state = 0; //reset state to line following
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

//update state_var
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
    
    //Initialize state variables, defined in state.h
    state_var = initializeState();
    
    while(1){
        updateState();
        stateMachine();
    }
    
    return 0;
}
