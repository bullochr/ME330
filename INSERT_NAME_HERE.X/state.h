
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATE
#define	STATE

#include <xc.h> // include processor files - each processor file is guarded.  

//Define a struct that will hold all of the state variables for the robot
struct stateStruct{
    int state_num; 
        //state 0 - line following
        //state 10 - navigate canyon
        //state 20 - collect sample
        //state 30 - data transmission
    
    //LEDs
    int led_1_num_blinks;
    int led_1_target_num_blinks;
    int led_2_num_blinks;
    int led_2_target_num_blinks;
    
    //wheel state
    int left_wheel_target_steps;
    int right_wheel_target_steps;
    int left_wheel_num_completed_steps;
    int right_wheel_num_completed_steps;
    float left_wheel_velocity;
    float right_wheel_velocity;
    float left_wheel_target_velocity; //target speed, actual speed approaches this in ramping up
    float right_wheel_target_velocity;
    
    //BALL INFO
    int ball_white;
    int turneddirection;

};

//contains custom flag states
struct flagStruct{
    int left_wheel_reached_target;
    int right_wheel_reached_target;
    int ad_conversions_finished;
};

//instantiate a state struct called state_var
struct stateStruct state;

//A global instance of flagStruct
struct flagStruct flags;

//fills state with default values, then returns filled state_var
struct stateStruct initializeState(void){
    state.state_num = 0;
    
    state.led_1_num_blinks = 0;
    state.led_1_target_num_blinks = 0;
    state.led_2_num_blinks = 0;
    state.led_2_target_num_blinks = 0;
    
    state.left_wheel_target_steps = 0;
    state.right_wheel_target_steps = 0;
    state.left_wheel_num_completed_steps = 0;
    state.right_wheel_num_completed_steps = 0;
    state.left_wheel_target_velocity = 0.0;
    state.right_wheel_target_velocity = 0.0;
    state.left_wheel_velocity = 0.0;
    state.right_wheel_velocity = 0.0;
    
    //ball stuff
    state.ball_white = -1; //initializes by stating the ball color is -1, changed later in Main, when checked by qrd
    state.turneddirection = -1; //initializes by making saved direction = -1
    
    return state;
}

struct flagStruct initializeFlags(void){
    flags.left_wheel_reached_target = 0;
    flags.right_wheel_reached_target = 0;
    flags.ad_conversions_finished = 0;
    
    return flags;
}

#endif	/* XC_HEADER_TEMPLATE_H */

