
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATE
#define	STATE

#include <xc.h> // include processor files - each processor file is guarded.  

//Define a struct that will hold all of the state variables for the robot
struct stateStruct{
    int state_num; 
        //state 0 - line following
            //state 1 - Milestone 5: basic motion
        //state 10 - collect sample
        //state 20 - navigate canyon
        //state 30 - data transmission
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
    int left_wheel_direction; //1 is forward, -1 is backward
    int right_wheel_direction; //1 is forward, -1 is backward
    int left_wheel_num_completed_steps;
    int right_wheel_num_completed_steps;
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
    state.left_line_sensor = 0;;
    state.right_line_sensor = 0;
    state.front_dist_sensor = 0;
    state.left_dist_sensor = 0;
    state.right_dist_sensor = 0;
    state.is_laser_on = 0;
    state.top_satellite_sensor = 0;
    state.bottom_satellite_sensor = 0;
    
    state.left_wheel_target_steps = 0;
    state.right_wheel_target_steps = 0;
    state.left_wheel_direction = 0;
    state.right_wheel_direction = 0;
    state.left_wheel_num_completed_steps = 0;
    state.right_wheel_num_completed_steps = 0;
    
    return state;
}

struct flagStruct initializeFlags(void){
    flags.left_wheel_reached_target = 0;
    flags.right_wheel_reached_target = 0;
    flags.ad_conversions_finished = 0;
    
    return flags;
}

#endif	/* XC_HEADER_TEMPLATE_H */

