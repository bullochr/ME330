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
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH);
                state.state_num = 2;
                
            } else if (allQrdsBlack()){ //check if the robot entered the canyon
                blinkLED2(2);
                resetWheelsState();
                driveForwardDistance(10); // drive straight a few steps and check if qrds are still black. 
                                            //This prevents false positives when crossing between tiles
                while(!bothWheelsFinished());
                
                if(allQrdsBlack()){
                    initializeCanyonNavigation();
                    state.state_num = 10;
                }else{
                    initializeLineFollowing();
                }
            }
            
            //TODO: update trigger condition, remove && 0
//            if (leftQrdOnLine() && centerQrdOnLine() && 0){ //Land trigger
//                turn90Degrees(0);
//                state.state_num = 20;
//            }
            if (center_qrd_value < CENTER_QRD_WHITE_THRESHOLD && sample_collection_value < PHOTO_DIODE_THRESHOLD){
                blinkLED1(3);
                turn90DegreesBlocking(0);
                setServoAngle(30);                         //ANGLE NEEDS ADJUSTING FOR BALL PICKUP
                driveDistanceBlocking(100);                //needs adjusting for distance to ball dispenser
                double timercount = 0;
                while (timercount <50000){
                    timercount++;
                }
                if (ball_qrd_value < WHITE_THRESHOLD){
                    state.ball_white = 1;
                    driveBackwardDistance(100);            //ditto needs adjusting
                    state.state_num = 30;
                }
                if ((ball_qrd_value > BLACK_THRESHOLD)){
                    state.ball_white = 0;
                    driveBackwardDistance(130);
                    state.state_num = 30;
                }
            }

            if (center_qrd_value < CENTER_QRD_WHITE_THRESHOLD && left_sharp_value > LEFT_SHARP_THRESH_DIST && right_sharp_value > RIGHT_SHARP_THRESH_DIST){
                blinkLED2(3);
                driveDistanceBlocking(100); //correction value if needed to get to the center of the bin after turning
                if (state.ball_white == 1){        //if ball is WHITE
                    turn90DegreesBlocking(1);
                    state.turneddirection = 1; //note that we have turned LEFT
                    driveDistanceBlocking(50);   //ADJUST TO REACH BIN
                    setServoAngle(20);
                    double servo_time = 0;
                    while (servo_time < 100000){
                        servo_time++;
                    }
                    setServoAngle(60);
                    driveBackwardDistance(30);  //ADJUST TO RETURN TO LINE
                    state.state_num = 31;     //set to ball deposit state
                }
                if (state.ball_white == 0){    //IF BALL BLACK
                    turn90DegreesBlocking(0);
                    state.turneddirection = 0;           //note that we have turned right
                    driveDistanceBlocking(30);            //ADJUST TO REACH BIN
                    setServoAngle(20);
                    double servo_time = 0;
                    while (servo_time < 100000){
                        servo_time++;
                    }
                    setServoAngle(60);
                    driveBackwardDistance(30);           //ADJUST TO RETURN TO LINE
                    state.state_num = 31;                 //set to ball deposit state
                }
                else {
                    
                }
            }            
            break;
            
        case 2: //both wheels are on the line, this state will turn the robot to prepare for line following
            if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
                setLeftWheelVelocity(-SLOW_WHEEL_VELOCITY);
                setRightWheelVelocity(SLOW_WHEEL_VELOCITY);

            } else{
                setLeftWheelVelocity(SLOW_WHEEL_VELOCITY);
                setRightWheelVelocity(-SLOW_WHEEL_VELOCITY);
            }    
            state.state_num = 3;
            break;
            
        case 3: 
            if(right_qrd_value > BLACK_THRESHOLD && center_qrd_value < WHITE_THRESHOLD && left_qrd_value > BLACK_THRESHOLD){
                resetWheelsState();
                initializeLineFollowing();
                state.state_num = 0;
            }
            break;
            
        case 10: //Canyon Forward
            if (allQrdsWhite()){
                driveDistanceBlocking(DIST_AXLE_TO_QRDS + LINE_WIDTH);
                state.state_num = 2;
//                blinkLED1(1);
            }
            if (front_sharp_value > FRONT_SHARP_THRESH_DIST){
                //right detects a wall, left doesnt
                if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value < LEFT_SHARP_THRESH_DIST){
//                    resetWheelsState();
                    turn90Degrees(0);
                    state.state_num = 11;
                    
                //right doesnt detect a wall, left does
                } else if (right_sharp_value < RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
//                    resetWheelsState();
                    turn90Degrees(1);
                    state.state_num = 11;
                
                //both walls detected
                }else if (right_sharp_value > RIGHT_SHARP_THRESH_DIST && left_sharp_value > LEFT_SHARP_THRESH_DIST){
//                    resetWheelsState();
                    driveForwardDistance(610);
                    state.state_num = 10;
                
                //no walls detected. This shouldnt happen
                }
            }      
            break;
            
        case 11: //Canyon Turn
            if (bothWheelsFinished()){
                setLeftWheelVelocity(BASE_WHEEL_VELOCITY);
                setRightWheelVelocity(BASE_WHEEL_VELOCITY);
                
                state.state_num = 10;
            }
        
         case 20: //Landing approach: finish left turn, start driving
            if (bothWheelsFinished()){
                driveForwardDistance(610);
                state.state_num = 21;
            }
            break;
            
         case 21: // Landing approach: forward until front sharp sees the wall
            if (front_sharp_value > FRONT_SHARP_THRESH_DIST){
                resetWheelsState();          // cancel the current forward move
                turn180Degrees(1);
                state.state_num = 23;        // skip 22, it's no longer needed
            }
            else if (bothWheelsFinished()){
                driveForwardDistance(610);   // keep going in chunks until the wall appears
            }
            break;
        
        case 23: //Landing approach: finish 180, begin landing zone
            if (bothWheelsFinished()){
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
            
        case 30: // BALL COLLECT BACK TO LINE FOLLOWING
            if (center_qrd_value > CENTER_QRD_WHITE_THRESHOLD){
                driveBackwardDistance(10);
            }
            else if (center_qrd_value <= CENTER_QRD_WHITE_THRESHOLD){
                //turn90DegreesBlocking(1);
            driveDistanceBlocking(DIST_AXLE_TO_QRDS);
                state.state_num = 2;
            }
            break;
        case 31: // BALL DEPOSIT BACK TO LINE FOLLOWING
            if (center_qrd_value > CENTER_QRD_WHITE_THRESHOLD) {
                driveBackwardDistance(10);
            }
            if (center_qrd_value <= CENTER_QRD_WHITE_THRESHOLD && state.turneddirection == 1){  //If hit the line and had previously turned left
                turn90DegreesBlocking(0);
                state.turneddirection = -1;
                state.state_num = 0;
            }
            if (center_qrd_value < CENTER_QRD_WHITE_THRESHOLD && state.turneddirection == 0){  //If hit the line and had previously turned left
                turn90DegreesBlocking(1);
                state.turneddirection = -1;
                state.state_num = 0;
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


