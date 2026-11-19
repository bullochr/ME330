
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GLOBAL_VARIABLE_DEFINITIONS
#define	GLOBAL_VARIABLE_DEFINITIONS

#include <xc.h> // include processor files - each processor file is guarded.  

//register definitions - inputs
#define left_qrd_value ADC1BUF4 
#define center_qrd_value ADC1BUF13 
#define right_qrd_value ADC1BUF14 //TODO: reset this to ADC1BUF14 (pin 8), figure out why pin 8 has so much noise, its currently on pin 2
#define satellite_sensor_value ADC1BUF0
#define ball_qrd_value ADC1BUF1
#define sample_collection_value ADC1BUF12
#define front_sharp_value ADC1BUF11
#define right_sharp_value ADC1BUF10
#define left_sharp_value ADC1BUF9


//register definitions - outputs
#define left_wheel_PWM_period OC1RS
#define right_wheel_PWM_period OC2RS
#define left_wheel_PWM_state OC1CON1bits.OCM
#define right_wheel_PWM_state OC2CON1bits.OCM
#define left_wheel_dir _LATB8
#define right_wheel_dir _LATB9
#define led_1_state _LATB4
#define led_2_state _LATA4
#define laser_state _LATB7
#define servo_pulse_width OC3R

//Global constants
const float WHEEL_DIAMETER_MM = 89.95; //mm
const int STEPS_PER_REV = 800; //quarter stepping
const float PI = 3.141592;
const float ROBOT_WIDTH_MM = 201.2; //distance from wheel to wheel
const int SERVO_PWM_PERIOD = 10000; //leave this at 10000 to keep pwm at 50hz
const int FRONT_SHARP_THRESH_DIST = 1500; // Front SHARP distance. ~6in Higher = closer, lower = farther away
const int LEFT_SHARP_THRESH_DIST = 500; // Left SHARP distance threshold. ~1.5 ft
const int RIGHT_SHARP_THRESH_DIST = 1000; // RIght SHARP distance threshold. ~1.5 ft

const int FORWARD = 0;
const int BACKWARD = 1;

const float MAX_WHEEL_VELOCITY = 2000.0;
const float MIN_WHEEL_VELOCITY = 70.0;
const int MAX_INTEGRAL_VALUE = 200;

const int BASE_WHEEL_PULSE_WIDTH = 299;
const float CLOCK_FREQUENCY = 4000000.0;
const int PWM_OFF = 0b000;
const int PWM_ON = 0b110;

const int WHITE_THRESHOLD = 1000; //qrd values below this are considered to be on the line 
const int BLACK_THRESHOLD = 2500; //qrd values above this are considered to be off the line 
const int CENTER_QRD_WHITE_THRESHOLD = 1000;
const int CENTER_QRD_BLACK_THRESHOLD = 2500;


const int LINE_WIDTH = 25;
const int DIST_AXLE_TO_QRDS = 45; // how far in front of the axle the qrd sensors are

//Line following
//const float BASE_WHEEL_VELOCITY = 1100.0; //fast
const float BASE_WHEEL_VELOCITY = 600.0; //testing
const float SLOW_WHEEL_VELOCITY = 400.0;
const float KP = 1.0; //proportional error coefficient for PID line control
const float KI = 0.0; //integral error coefficient for PID line control
const float KD = 0.5; //derivative error coefficient for PID line control
const float SLOW_DOWN_COEFFICIENT = 0.2; //what portion of correction factor is applied to slow down the robot while turning
                                        //the sharper the robot is turning, the slower it will go
const float RAMP_UP_SPEED = 15; //slope of ramp up function. Higher value is more 
                             //responsive, lower value is less likely to skip steps



#endif	/* XC_HEADER_TEMPLATE_H */

