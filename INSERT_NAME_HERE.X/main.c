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
//                left_wheel_PWM_period = BASE_WHEEL_PERIOD - correction;
//                right_wheel_PWM_period = BASE_WHEEL_PERIOD + correction;
                
                
                left_wheel_PWM_period = right_wheel_PWM_period = BASE_WHEEL_PERIOD;
                
                if (leftQrdOnLine() && centerQrdOnLine()){ //Land trigger
                //if (left_sharp_value > 150){
                    left_wheel_PWM_period = BASE_WHEEL_PERIOD;
                    right_wheel_PWM_period = BASE_WHEEL_PERIOD;
                    resetWheelsState();
                    turn90Degrees(0);
                    state.state_num = 20;
                }
                break;
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
                    state.state_num = 30;
                }else{
                    turn90Degrees(0);
                    state.state_num = 30;
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
            break;
            
        case 20: //Landing approach: finish left turn, start driving
            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(610);
                state.state_num = 21;
            }
            break;

//        case 21: //Landing approach: forward until both walls are seen
//            if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
//                resetWheelsState();
//                driveForwardDistance(LAND_STEPS_MM);
//                led_2_state = 1;
//                state.state_num = 22;
//            } 
//            else if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
//            resetWheelsState();
//            driveForwardDistance(610);
//            }
//            break;
            
        case 21: // Landing approach: forward until front sharp sees the wall
            if (front_sharp_value > FRONT_SHARP_THRESH_DIST){
                resetWheelsState();          // cancel the current forward move
                turn180Degrees(1);
                state.state_num = 23;        // skip 22, it's no longer needed
            }
            else if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(610);   // keep going in chunks until the wall appears
            }
            break;
            
//        case 22: //Landing approach: finish forward steps, turn 180
//            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
//                resetWheelsState();
//                turn180Degrees(1);
//                state.state_num = 23;
//            }
//            break;

        case 23: //Landing approach: finish 180, begin landing zone
            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                state.state_num = 24;
            }
            break;
            
        case 24: //Landing Zone
            if (satellite_sensor_value < IR_SATELITE_THRESH){
                
                static float servo_angle = 0;
                
                setServoAngle(servo_angle);
                servo_angle = servo_angle + .01;
                
                if (satellite_sensor_value > 200){ //use to test IR Threshold needed
                    led_1_state = 1;}
                
                if (satellite_sensor_value > 100){ //use to test IR Threshold needed
                    led_2_state = 1;}
                
                if (servo_angle > 180){
                    servo_angle = 0;
                    led_1_state = 0;
                    led_2_state = 0;
                }
            }
            
            if (satellite_sensor_value > IR_SATELITE_THRESH){

               
                laser_state = 1;
                state.state_num = 25;
            }
            break;
            
        case 25: //Laser State
            
            //Do nothing
            break;
            
        case 30://New Testing Code State
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
            
        case 40:
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
    
    state.state_num = 0; 
    
    initializeLineFollowing();
//    initializeCanyonNavigation();
        
    laser_state = 0;
    while(1){
//        updateState();
        stateMachine();
    }
    
    return 0;
}
