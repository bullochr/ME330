#include "xc.h"
#include "global_variables.h" //includes all global vaariables defined in the header file with this name
#include "ISR_functions.h" //includes all interrupt service routine function definitions in the header file with this name
#include "driving.h" //all functions related to motion of the robot
#include "pin_config.h" //configure pin settings
#include "state.h" //defines state struct and flag struct and functions to initialize them
#include "AD_Config.h" //configure analog to digital conversions
#include "PWM_config.h"
#include "bucket_servo.h"

//Oscillator setup
#pragma config FNOSC = FRC //F_osc = 8MHz, F_cy = 4MHz 
#pragma config OSCIOFNC = OFF //makes pin 8 not output the clock so we can use it as input
#pragma config SOSCSRC = DIG //allows us to use pins 9 and 10 as output

int correction = 0;

void stateMachine(){
    switch(state.state_num){
        case 0: //line following
//            left_wheel_PWM_period = BASE_WHEEL_SPEED - center_qrd_value;
//            right_wheel_PWM_period = BASE_WHEEL_SPEED + center_qrd_value;
            if (flags.ad_conversions_finished){
                flags.ad_conversions_finished = 0;
                int correction = getCorrectionFactor();
                
                //apply correction factor to wheel speeds
                left_wheel_PWM_period = BASE_WHEEL_PERIOD - correction;
                right_wheel_PWM_period = BASE_WHEEL_PERIOD + correction;
            }
            
//            if(allQrdsOnLine()){//check if the robot ran into a T in the line
//                //transition to state 1
//                driveForwardDistance(DIST_AXLE_TO_QRDS + (LINE_WIDTH / 2));//forward until axle is over the line
//                state.state_num = 1;
//            }
            break;
            
        case 1: // Ran perpendicular into the line, or a T in the line
            if(flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                turn90Degrees(1); //turn left 90 degrees to align with the line
                state.state_num = 2;
            }
            break;
            
        case 2:
            if(center_qrd_value < WHITE_THRESHOLD){//check if the center qrd is on the line
                resetWheelsState();
                state.state_num = 0; //back to line following
            } else if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){ //if finished 90 degrees and still didnt intersect line
                resetWheelsState();
                turn90Degrees(1); //keep turning left
            }
            break;
            
        case 10: //Canyon Forward 
            if (allQrdsOnLine()){
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST){
                    turn90Degrees(1);
                    state.state_num = 20;
                }else{
                    turn90Degrees(0);
                    state.state_num = 20;
                }
            }
            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
                    led_1_state = 1;
                    led_2_state = 0;
                    turn90Degrees(1);
                    state.state_num = 11;
                } else if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                    led_1_state = 0;
                    led_2_state = 1;
                    turn90Degrees(0);
                    state.state_num = 11;
                }else{
                    led_1_state = 1;
                    led_2_state = 1;
                    driveForwardDistance(610);
                    state.state_num = 10;
                }
            }
                
//            if (left_sharp_value > LEFT_SHARP_THRESH_DIST && right_sharp_value > RIGHT_SHARP_THRESH_DIST){
//                resetWheelsState();
//                led_1_state = 1;
//                led_2_state = 1;
//                CanyonDriveForward(300);
//            } else if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
//                resetWheelsState();
//                turn90Degrees(1);
//                led_1_state = 1;
//                led_2_state = 0;
//                CanyonDriveForward(300);
//            } else if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
//                resetWheelsState();
//                turn90Degrees(0);
//                led_1_state = 0;
//                led_2_state = 1;
//                CanyonDriveForward(300);
//            }
////            if(center_qrd_value < WHITE_THRESHOLD){
//                initializeLineFollowing();
//            }           
            break;
            
        case 11: //Canyon Turn
            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(610);
                state.state_num = 10;
            }
        
        case 20://New Testing Code State
//          reset outputs to test
//            led_1_state = 0;
//            led_2_state = 0;
//            laser_state = 0;
//            servo_pulse_width = 0;
//            
            //            if (satellite_sensor_value > 2000){
//                laser_state = 1;
//            } 
//            if (ball_qrd_value < 2000){
//                servo_pulse_width = SERVO_PWM_PERIOD / 2;
//            }
//            if (sample_collection_IR > 2000){
//                servo_pulse_width = SERVO_PWM_PERIOD;
//            }
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
    //configure setup
    configurePins();
    configureAD();
    configurePWM();
    
    //initialize a struct to hold the states of each custom flag
    flags = initializeFlags();
    
    //initialize a struct to hold the variables that define the state of the robot
//    state = initializeState();
    
    state.state_num = 10; 
    
//    initializeLineFollowing();
    initializeCanyonNavigation();
        
//    laser_state = 1;
    while(1){
//        updateState();
        stateMachine();
    }
    
    return 0;
}
