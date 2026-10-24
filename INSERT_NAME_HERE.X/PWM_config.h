
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_config
#define	PWM_config

#include <xc.h> // include processor files - each processor file is guarded.  
#include "global_variables.h"

void configurePWM(void){
    //clear PWM registers
    OC1CON1 = 0; //Pin 14
    OC1CON2 = 0;
    OC2CON1 = 0; //Pin 4
    OC2CON2 = 0;
    OC3CON1 = 0; //Pin 5
    OC3CON2 = 0;
    
    //PWM pins with interrupts for wheels (14 and 4)
    OC1CON1bits.OCTSEL = 0b111; //set PWM on pin 14 (left wheel) to use internal system clock
    OC2CON1bits.OCTSEL = 0b111; //set PWM on pin 4 (right wheel) to use internal system clock
    
    OC1CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 14
    OC2CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 4
    
    OC1CON2bits.SYNCSEL = 0x1F; //configure PWM module sync source on pin 14
    OC2CON2bits.SYNCSEL = 0x1F; //configure PWM module sync source on pin 4
    
    OC1CON2bits.OCTRIG = 0; //not sure what this does exactly
    OC2CON2bits.OCTRIG = 0;
    
    left_wheel_PWM_period = BASE_WHEEL_SPEED; //set the period of PWM on PIN 14 in clock cycles
    right_wheel_PWM_period = BASE_WHEEL_SPEED; //set the period of PWM on PIN 4 in clock cycles
    
    OC1R = BASE_WHEEL_PULSE_WIDTH; //set duty cycle
    OC2R = BASE_WHEEL_PULSE_WIDTH; 
    
    _OC1IP = 4; //set pin 14 interrupt priority to 4
    _OC2IP = 4; //set pin 4 interrupt priority to 4
    
    _OC1IE = 1; //enable interrupt on pin 14
    _OC2IE = 1; //enable interrupt on pin 4
    
    _OC1IF = 0; //clear flag on pin 14 
    _OC2IF = 0; //clear flag on pin 4 
    
    //PWM for servo motor (5)
    OC3CON1bits.OCTSEL = 0b111; //set PWM on pin 5 (servo) to use internal system clock
    
    OC3CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 5
    
    OC3CON2bits.SYNCSEL = 0x1F; //configure PWM module sync source on pin 5
    
    OC3CON2bits.OCTRIG = 0; //not sure what this does exactly
    
    OC3RS = 65535; //TODO: The period should be 50Hz (see datasheet), but that 
    //requires 80000 counts with 8MHz osc, which overflows the 16bit register
    //we could get 61Hz using OC3RS = 65535
    //or use an internal timer with a prescaler
    
    OC3R = 0; //TODO: set duty cycle to define initial position of servo
    
}

#endif	/* XC_HEADER_TEMPLATE_H */

