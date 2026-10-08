/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_STATE
#define	XC_STATE

#include <xc.h> // include processor files - each processor file is guarded.  

//Define a struct that will hold all of the state variables for the robot
struct stateStruct{
    int state; 
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
    int left_wheel_completed_steps;
    int right_wheel_completed_steps;
    int left_wheel_reached_target;
    int right_wheel_reached_target;
};

//instantiate a state struct called state_var
struct stateStruct state_var;

//fills state with default values, then returns filled state_var
struct stateStruct initializeState(void){
    state_var.state = 1;
    state_var.left_line_sensor = 0;;
    state_var.right_line_sensor = 0;
    state_var.front_dist_sensor = 0;
    state_var.left_dist_sensor = 0;
    state_var.right_dist_sensor = 0;
    state_var.is_laser_on = 0;
    state_var.top_satellite_sensor = 0;
    state_var.bottom_satellite_sensor = 0;
    
    state_var.left_wheel_target_steps = 0;
    state_var.right_wheel_target_steps = 0;
    state_var.left_wheel_direction = 0;
    state_var.right_wheel_direction = 0;
    state_var.left_wheel_completed_steps = 0;
    state_var.right_wheel_completed_steps = 0;
    state_var.left_wheel_reached_target = 0;
    state_var.right_wheel_reached_target = 0;
    
    return state_var;
}

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

