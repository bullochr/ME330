
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIN_CONFIG
#define	PIN_CONFIG

#include <xc.h> // include processor files - each processor file is guarded.  
#include "global_variables.h"

void configurePins(void){
    //clear all registers
    ANSA = 0;
    ANSB = 0;
    TRISA = 0;
    TRISB = 0;
    LATA = 0;
    LATB = 0;
    
    //Digital output pins for stepper DIR (12, 13)
    _TRISB8 = 0; //configure pin 12 (right wheel) for output
    _TRISB9 = 0; //configure pin 13 (left wheel) for output
    
    _LATB8 = 0; //set latch to 0
    _LATB9 = 0;
    
    //Analog input for ball color QRD (3)
    _TRISA1 = 1; //set to input
    _ANSA1 = 1; //set to analog
    
    //Analog input for line following (6,7, 8)
    _TRISB2 = 1; //set pin 6 to input (left of line sensor)
    _TRISA2 = 1; //set pin 7 to input (on line sensor)
    _TRISA3 = 1; // right of line qrd
    
    _ANSB2 = 1; //set pin 6 to analog
    _ANSA2 = 1; //set pin 7 to analog
    _ANSA3 = 1; //pin 8, right of line
    
    //Analog input for distance sensors, front (16), right (17), and left (18)
    _TRISB13 = 1; //set pin 16 to input
    _TRISB14 = 1; //set pin 17 to input
    _TRISB15 = 1; //set pin 18 to input
    
    _ANSB13 = 1; //set pin 16 to analog
    _ANSB14 = 1; //set pin 17 to analog
    _ANSB15 = 1; //set pin 18 to analog
    
    //Analog input for satellite IR sensor (2)
    _TRISA0 = 1; //set to input
    _ANSA0 = 1; //set to analog
    
    //Analog input for sample collection site (15)
    _TRISB12 = 1; //set to input
    _ANSB12 = 1; //set to analog
}

#endif	/* XC_HEADER_TEMPLATE_H */

