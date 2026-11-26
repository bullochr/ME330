#include "xc.h"
#include "global_variables.h" //includes all global vaariables defined in the header file with this name
#include "ISR_functions.h" //includes all interrupt service routine function definitions in the header file with this name
#include "driving.h" //all functions related to motion of the robot
#include "pin_config.h" //configure pin settings
#include "state.h" //defines state struct and flag struct and functions to initialize them
#include "AD_Config.h" //configure analog to digital conversions
#include "PWM_config.h"
#include "bucket_servo.h"
#include "LEDs.h"
#include "wait.h"
#include "ball_collection.h"
#include "task_integration.h"

//Oscillator setup
#pragma config FNOSC = FRC //F_osc = 8MHz, F_cy = 4MHz 
#pragma config OSCIOFNC = OFF //makes pin 8 not output the clock so we can use it as input
#pragma config SOSCSRC = DIG //allows us to use pins 9 and 10 as output

int correction = 0;

void stateMachine(){
    switch(state.state_num){
        case 0: //line following
            if (flags.ad_conversions_finished){
                flags.ad_conversions_finished = 0;
                followLine();
            }
            
            if(allQrdsWhite()){//check if the robot ran into a T in the line
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH, SLOW_WHEEL_VELOCITY);
                state.state_num = 2;
                
            } else if (allQrdsBlack()){ //check if the robot entered the canyon
                resetWheelsState();
                driveDistanceBlocking(10, BASE_WHEEL_VELOCITY); // drive straight a few steps and check if qrds are still black. 
                                            //This prevents false positives when crossing between tiles

                if(allQrdsBlack()){
                    if(state.finished_canyon == 0){
                        initializeCanyonNavigation();
                        state.state_num = 10;
                    } else if (state.all_tasks_finished){
                        resetWheelsState();          // cancel the current forward move
                        turn90DegreesBlocking(1);
                        turn90DegreesBlocking(1);
                        driveDistanceBlocking(50, SLOW_WHEEL_VELOCITY);
                        state.state_num = 24;
                    }
                }else{
                    initializeLineFollowing();
                    state.state_num = 0;
                }
            }

            //transition to sample collection
            //only if ball hasn't been collected yet
            if (state.collected_sample == 0 && center_qrd_value < WHITE_THRESHOLD && sample_collection_value < PHOTO_DIODE_THRESHOLD){
                blinkLED1(3);
                driveDistanceBlocking(DIST_BEFORE_SAMPLE_COLLECTION, BASE_WHEEL_VELOCITY);
                turn90DegreesBlocking(1);
                setServoAngle(SMAPLE_COLLECT_SERVO_ANGLE);                         //ANGLE NEEDS ADJUSTING FOR BALL PICKUP
                driveDistanceBlocking(DIST_TO_SAMLE_COLLECTION, BASE_WHEEL_VELOCITY);                //needs adjusting for distance to ball dispenser
                waitMsBlocking(1000);
                setServoAngle(MAX_SERVO_ANGLE); //increase servo angle to keep ball in 
                waitMsBlocking(1500); //wait for bouncing to settle before measuring color
                if (ball_qrd_value <= BALL_COLOR_THRESHOLD){
                    state.ball_white = 1;
                    led_2_state = 0;
                    led_1_state = 1;
                }
                else if ((ball_qrd_value > BALL_COLOR_THRESHOLD)){
                    state.ball_white = 0;
                    led_1_state = 0;
                    led_2_state = 1;
                }
                resetWheelsState();
                setLeftWheelVelocity(-SLOW_WHEEL_VELOCITY);
                setRightWheelVelocity(-SLOW_WHEEL_VELOCITY);
                state.collected_sample = 1;
                state.state_num = 30;
            }

            if (state.collected_sample == 1 && state.deposited_sample == 0 && center_qrd_value < WHITE_THRESHOLD && left_sharp_value > LEFT_SHARP_DEPOSIT_DIST && right_sharp_value > RIGHT_SHARP_DEPOSIT_DIST){
                blinkLED2(3); //correction value if needed to get to the center of the bin after turning
                depositSample();
                state.deposited_sample = 1;
                state.state_num = 30;  
            }
            break;
            
        case (2): //both wheels are on the line, this state will turn the robot to prepare for line following
            ; //this prevents a compiler error due to a declaration following a label
            int turn_left = 0;
            if (state.turneddirection != -1){ //this means we are returning from ball deposition
                if (state.turneddirection == 1){  //If hit the line and had previously turned left
                    turn_left = -1;
                    state.turneddirection = -1;
                }
                
                else if (state.turneddirection == 0){  //If hit the line and had previously turned left
                    turn_left = 1;
                    state.turneddirection = -1;
                }
                
            }else{
                if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                    turn_left = 1;
                } 
                
                else{
                    turn_left = -1;
                }    
            }
            
            setLeftWheelVelocity(-turn_left * SLOW_WHEEL_VELOCITY);
            setRightWheelVelocity(turn_left * SLOW_WHEEL_VELOCITY);
            state.state_num = 3;
            break;
            
        case 3: 
            if(right_qrd_value > LOOSE_BLACK_THRESHOLD && center_qrd_value < WHITE_THRESHOLD && left_qrd_value > LOOSE_BLACK_THRESHOLD){
                resetWheelsState();
                initializeLineFollowing();
                if (state.deposited_sample){
                    state.all_tasks_finished = 1;
                }
                state.state_num = 0;
            }
            break;
            
        case 10: //Canyon Forward
            if (noQrdsBlack()){
                led_1_state = 1;
                led_2_state = 1;
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH, BASE_WHEEL_VELOCITY);
                state.state_num = 2;
                state.finished_canyon = 1;
            }
            
            if(state.num_canyon_tiles_finished >= 3){
                setLeftWheelVelocity(SLOW_WHEEL_VELOCITY);
                setRightWheelVelocity(SLOW_WHEEL_VELOCITY);
            }else{
                state.num_canyon_tiles_finished++;

                //right detects a wall, left doesnt
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
                    turn90DegreesBlocking(0);

                //right doesnt detect a wall, left does
                } else if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                    turn90DegreesBlocking(1);
                }

                driveDistanceBlocking(TILE_WIDTH, CANYON_WHEEL_VELOCITY);
            }
            break;
            
        case 24: //Landing Zone
            ;
            static int best_servo_angle = 0;
            static int highest_sensor_value = 0; //more IR light -> lower sensor value
//            static int first_saturated_angle = 0;
            static float servo_angle = 20;
            
            if (servo_angle < MAX_SERVO_ANGLE+10){
                                
                setServoAngle((int)servo_angle);
                waitMsBlocking(50);
                
                if(satellite_sensor_value > highest_sensor_value){
//                    if (satellite_sensor_value > 3900 && first_saturated_angle == 0){
//                        first_saturated_angle = servo_angle;
//                    }
                    highest_sensor_value = satellite_sensor_value;
                    best_servo_angle = servo_angle;
                }
                
                servo_angle = servo_angle + 1;
                
                
            } else{
                
                //if the sensor was saturated, find midpoint of saturated values
//                if(first_saturated_angle != 0){
//                    best_servo_angle = (best_servo_angle + first_saturated_angle) / 2;
//                }
                setServoAngle(best_servo_angle);
                waitMsBlocking(500); //allow servo time to reach best_servo_angle
                laser_state = 1;
                state.state_num = 25;
            }
            break;
            
        case 25: //Laser State
            //Do nothing
            break;
            
        case 30: // BALL COLLECT BACK TO LINE FOLLOWING
//            if (center_qrd_value > WHITE_THRESHOLD){
//                driveDistanceBlocking(-10);
//            }else 
            if (noQrdsBlack()){
                driveDistanceBlocking(DIST_AXLE_TO_QRDS+10, BASE_WHEEL_VELOCITY);
                state.state_num = 2;
            }
            break;
        
        default:
            break;
    }
}

int main(void) {
    //configure setup
    configurePins();
    configureAD();
    configurePWM();
    
    T1CONbits.TON = 1;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS = 0;
    TMR1 = 0;
    
    
    //initialize a struct to hold the states of each custom flag
    flags = initializeFlags();
    
    //initialize a struct to hold the variables that define the state of the robot
    state = initializeState();
    
    state.state_num = 0;
    
    initializeLineFollowing();
     
    while(1){ 
       if (state.state_num != 0){
           updateWheelVelocity();
       }
       stateMachine();
    }
    return 0;
}


