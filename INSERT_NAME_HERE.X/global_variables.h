
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
#define right_qrd_value ADC1BUF0 //TODO: reset this to ADC1BUF14 (pin 8), figure out why pin 8 has so much noise, its currently on pin 2
#define satellite_sensor_value ADC1BUF0
#define ball_qrd_value ADC1BUF1
#define sample_collection_IR ADC1BUF12
#define front_sharp_value ADC1BUF11
#define right_sharp_value ADC1BUF10
#define left_sharp_value ADC1BUF9


//register definitions - outputs
#define left_wheel_PWM_period OC1RS
#define right_wheel_PWM_period OC2RS
#define left_wheel_PWM_state OC1CON1bits.OCM
#define right_wheel_PWM_state OC2CON1bits.OCM
#define led_1_state _LATB4
#define led_2_state _LATA4
#define laser_state _LATB7
#define servo_pulse_width OC3R

//Global constants
const float WHEEL_DIAMETER_MM = 89.95; //mm
const int STEPS_PER_REV = 800; //quarter stepping
const float PI = 3.141592;
const float ROBOT_WIDTH_MM = 235.0; //distance from wheel to wheel
const int SERVO_PWM_PERIOD = 65535;

//const double KP = 1.0; //proportional error coefficient for PID line control

const double KP = 1000.0; //proportional error coefficient for PID line control
const float KI = 0.0; //integral error coefficient for PID line control
const float KD = 0.0; //derivative error coefficient for PID line control
const int FORWARD = 0;
const int BACKWARD = 1;

const int BASE_WHEEL_SPEED = 6000;
const int MAX_CORRECTION_VALUE = 5000;
const int BASE_WHEEL_PULSE_WIDTH = 299;
const int PWM_OFF = 0b000;
const int PWM_ON = 0b110;

#define QRD_FILTER_LENGTH 50;
//const static int QRD_FILTER_LENGTH = 50; //line following averages the last n measurements


#endif	/* XC_HEADER_TEMPLATE_H */

