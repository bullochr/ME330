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
                //turn on led to signal state change
//                blinkLED1(1);
                led_1_state = 1;
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH);  
                led_1_state = 0;
                state.state_num = 2;
//                
            } else if (allQrdsBlack()){ //check if the robot entered the canyon
//                blinkLED2(2);
//                initializeCanyonNavigation();
//                state.state_num = 10;
            }
            break;
            
        case 2: //both wheels are on the line, this state will turn the robot to prepare for line following
            
//            if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
//                blinkLED1(1);
//                setLeftWheelVelocity(-SLOW_WHEEL_VELOCITY);
//                setRightWheelVelocity(SLOW_WHEEL_VELOCITY);

//            } else{
            led_2_state = 1;
                setLeftWheelVelocity(SLOW_WHEEL_VELOCITY);
                setRightWheelVelocity(-SLOW_WHEEL_VELOCITY);
//            }    
            state.state_num = 3;
            break;
            
        case 3: 
            if(right_qrd_value > BLACK_THRESHOLD && center_qrd_value < WHITE_THRESHOLD && left_qrd_value > BLACK_THRESHOLD){
                led_2_state = 0;
                initializeLineFollowing();
                state.state_num = 0;
            }
            break;
            
        case 10: //Canyon Forward
            if (allQrdsWhite()){
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH);
                state.state_num = 2;
                blinkLED1(1);
            }
            if (front_sharp_value > FRONT_SHARP_THRESH_DIST){
                //right detects a wall, left doesnt
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
                    resetWheelsState();
                    led_1_state = 1;
                    led_2_state = 0;
                    turn90Degrees(0);
                    state.state_num = 11;
                    
                //right doesnt detect a wall, left does
                } else if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                    resetWheelsState();
                    led_1_state = 0;
                    led_2_state = 1;
                    turn90Degrees(1);
                    state.state_num = 11;
                
                //both walls detected
                }else if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                    resetWheelsState();
                    led_1_state = 1;
                    led_2_state = 1;
                    driveForwardDistance(610);
                    state.state_num = 10;
                
                //no walls detected. This shouldnt happen
                }else {
                    //blink red led rapidly 5 times
                    errorLED2(5, 1000);
                }
            }      
            break;
            
        case 11: //Canyon Turn
            if (bothWheelsFinished()){
                setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
                setRightWheelVelocity(BASE_WHEEL_VELOCITY);
                
                state.state_num = 10;
            }
        
        case 20://

            break;
            
        case 30:
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
    
//    state.state_num = 1;
    
    initializeLineFollowing();
    
    while(1){ 
        if (state.state_num != 0){
            updateWheelVelocity();
        }
        
        if(right_qrd_value > BLACK_THRESHOLD && center_qrd_value < WHITE_THRESHOLD && left_qrd_value > BLACK_THRESHOLD){
            led_2_state = 1;
        }else {
            led_2_state = 0;
        }
        
        stateMachine();
    }
    
    return 0;
}
