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
//    if (state.state_num != 0){ //use ramping function when changing speeds outside of line following
    updateWheelVelocity();
//    }
    
    switch(state.state_num){
        case 0: //line following
            led_2_state = 0;
            if (flags.ad_conversions_finished){
                flags.ad_conversions_finished = 0;
                followLine();
                
                led_2_state = 1;
            }
            
            if(allQrdsWhite()){//check if the robot ran into a T in the line
                flags.left_wheel_reached_target = 0;
                flags.right_wheel_reached_target = 0;
                driveForwardDistance(DIST_AXLE_TO_QRDS + LINE_WIDTH);
                
                //turn on led to signal state change
                blinkLED1(3);
                
                state.state_num = 2;
                
            } else if (allQrdsBlack()){ //check if the robot entered the canyon
                blinkLED2(2);
                initializeCanyonNavigation();
                state.state_num = 10;
            }
            break;
            
        case 2: //wait until finished driving before turning
            if(bothWheelsFinished()){
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
                    blinkLED1(1);
                    turn90Degrees(1);
                    
                } else{
                    blinkLED2(1);
                    turn90Degrees(0);
                }    
                state.state_num = 3;
            }
            break;
            
        case 3: 
//            if(center_qrd_value < CENTER_QRD_WHITE_THRESHOLD){
            if (bothWheelsFinished()){
                blinkLED2(2);
                initializeLineFollowing();
                state.state_num = 0;
            }
            break;
            
        case 10: //Canyon Forward 
            if (allQrdsWhite()){
                driveForwardDistance(DIST_AXLE_TO_QRDS + LINE_WIDTH);
                state.state_num = 2;
            }
            if (bothWheelsFinished()){
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
            if (bothWheelsFinished()){
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
        stateMachine();
    }
    
    return 0;
}
