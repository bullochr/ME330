#include "xc.h"

//Global constants
int WHEEL_DIAMETER_MM = 50; //not an actual measurement
int STEPS_PER_REV = 200; //just a guess
float PI = 3.141592;
int ROBOT_WIDTH_MM = 200; //distance from wheel to wheel, currently a guess

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
    float top_satellite_sensor;
    float bottom_satellite_sensor;
    
    //wheel state
    int left_wheel_target_steps;
    int right_wheel_target_steps;
    int left_wheel_direction; //1 is forward, 0 is backward
    int right_wheel_direction; //1 is forward, 0 is backward
    int left_wheel_completed_steps;
    int right_wheel_completed_steps;
};

//instantiate a state struct called state_var
struct stateStruct state_var;

//ISR for left wheel PWM. On each falling edge, add one to state_var.left_wheel_completed_steps and check if it has reached state_var.left_wheel_target_steps

//ISR for right wheel PWM. On each falling edge, add one to state_var.right_wheel_completed_steps and check if it has reached state_var.right_wheel_target_steps

//return number of steps for a wheel to take, given the desired distance traveled
int getNumSteps(int dist_mm){
    return (int)(dist_mm * (STEPS_PER_REV / (PI * WHEEL_DIAMETER_MM)));
}

//make a 90 degree turn. Pass a 1 to turn left, or a 0 to turn right
void turn90Degrees(int turn_left){
    int num_steps = getNumSteps(0.25*PI*ROBOT_WIDTH_MM); //calculates num_steps to travel 1/4 circumference of a circle with diam = ROBOT_WIDTH
    if(turn_left){
        return; 
    }else{
        return;
    }
}

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
    state_var.top_satellite_sensor = 0;
    state_var.bottom_satellite_sensor = 0;
    
    while(1){
        updateState();
        stateMachine();
    }
    
    return 0;
}
