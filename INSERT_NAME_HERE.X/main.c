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
            
            if(allQrdsOnLine()){//check if the robot ran into a T in the line
//                resetWheelsState();
//                setLeftWheelVelocity(-BASE_WHEEL_VELOCITY);
//                setRightWheelVelocity(-BASE_WHEEL_VELOCITY);
                
                //turn on led to signal state change
//                blinkLED1(3);
                
//                state.state_num = 30;
            } else if (allQRDsBlack()){ //check if the robot entered the canyon
//                blinkLED1(3);
//                initializeCanyonNavigation();
//                state.state_num = 10;
            }
            break;
        
        case 1: // Ran perpendicular into the line, or a T in the line
            //drive straight until the QRDs are off the line
            if(allQRDsBlack()){
                driveForwardDistance(DIST_AXLE_TO_QRDS);
                state.state_num = 2;
            }
            break;
            
        case 2: //wait until finished driving before turning
            if(bothWheelsFinished()){  
                resetWheelsState();
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
                    led_1_state = 1;
                    led_2_state = 0;
                    turn90Degrees(1);
                    
                } else{
                    led_1_state = 0;
                    led_2_state = 1;
                    turn90Degrees(0);
                }    
                state.state_num = 3;
            }
            break;
            
        case 3: 
            if (bothWheelsFinished()){
                initializeLineFollowing();
                state.state_num = 0;
            }
            break;
            
        case 10: //Canyon Forward 
            if (allQrdsOnLine()){
                resetWheelsState();
                led_1_state = 0;
                led_2_state = 0;
                state.state_num = 1;
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
            break;
            
        case 11: //Canyon Turn
            if (flags.left_wheel_reached_target && flags.right_wheel_reached_target){
                resetWheelsState();
                driveForwardDistance(610);
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
    
    T1CONbits.TON = 1;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS = 0;
    TMR1 = 0;
    
    //initialize a struct to hold the states of each custom flag
    flags = initializeFlags();
    
    //initialize a struct to hold the variables that define the state of the robot
    state = initializeState();
    
    state.state_num = 0;     
    
    setRightWheelVelocity(BASE_WHEEL_VELOCITY);
    setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
   
    
    while(1){
        updateWheelVelocity();
//        stateMachine();
    }
    
    return 0;
}
