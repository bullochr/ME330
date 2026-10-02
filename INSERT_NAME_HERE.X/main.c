/*
 * File:   main.c
 * Author: richa
 *
 * Created on October 2, 2026, 12:20 PM
 */


#include "xc.h"

//Define a struct that will hold all of the state variables for the robot
struct stateStruct{
    int state; 
        //state 0 - line following
        //state 1 - collect sample
        //state 2 - navigate canyon
        //state 3 - data transmission
    float left_line_sensor;
    float right_line_sensor;
    float front_dist_sensor;
    float left_dist_sensor;
    float right_dist_sensor;
    float is_laser_on;
    float top_satelite_sensor;
    float bottom_satelite_sensor;
};

//instantiate a state struct called state_var
struct stateStruct state_var;

void stateMachine(){
    if(state_var.state == 0){ //line following
        //if left sensor is low(black), turn right
        //if right sensor is high(white), turn left
        continue;
    }else if(state_var.state == 1){
        continue;
    }else if(state_var.state == 2){
        continue;
    }else if(state_var.state == 3){
        continue;
    }
}

//update state_var
void updateState(){
    //get sensor data
    //make state change decisions
    continue;
}

int main(void) {
    //Configure pins
    
    //Reset registers
    
    //Initialize state variables
    state_var.state = 0;
    state_var.left_line_sensor = 0;;
    state_var.right_line_sensor = 0;
    state_var.front_dist_sensor = 0;
    state_var.left_dist_sensor = 0;
    state_var.right_dist_sensor = 0;
    state_var.is_laser_on = 0;
    state_var.top_satelite_sensor = 0;
    state_var.bottom_satelite_sensor = 0;
    
    while(1){
        updateState();
        stateMachine();
    }
    
    return 0;
}
