/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

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

void configure_pins(void){
    //clear all registers
    ANSA = 0;
    ANSB = 0;
    TRISA = 0;
    TRISB = 0;
    LATA = 0;
    LATB = 0;
    
    //PWM pins with interrupts for wheels (14 and 4)
    OC1CON1bits.OCTSEL = 0b111; //set PWM on pin 14 (left wheel) to use internal system clock
    OC2CON1bits.OCTSEL = 0b111; //set PWM on pin 4 (right wheel) to use internal system clock
    
    OC1CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 14
    OC2CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 4
    
    OC1CON2bits.SYNCSEL = 0x1F; //configure PWM module sync source on pin 14
    OC2CON2bits.SYNCSEL = 0x1F; //configure PWM module sync source on pin 4
    
    OC1CON2bits.OCTRIG = 0; //not sure what this does exactly
    OC2CON2bits.OCTRIG = 0;
    
    OC1RS = 6000; //set the period of PWM on PIN 14 in clock cycles
    OC2RS = 6000; //set the period of PWM on PIN 4 in clock cycles
    
    OC1R = 599; //10% Duty cycle on pin 14
    OC2R = 599; //10% Duty cycle on pin 4
    
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
    
    //Digital output pins for stepper DIR (12, 13)
    _TRISB8 = 0; //configure pin 12 (right wheel) for output
    _TRISB9 = 0; //configure pin 13 (left wheel) for output
    
    _LATB8 = 0; //set latch to 0
    _LATB9 = 0;
    
    //Analog input for ball color QRD (3)
    _TRISA1 = 1; //set to input
    _ANSA1 = 1; //set to analog
    
    //Analog input for line following (6,7)
    _TRISB2 = 1; //set pin 6 to input (left of line sensor)
    _TRISA2 = 1; //set pin 7 to input (on line sensor)
    
    _ANSB2 = 1; //set pin 6 to analog
    _ANSA2 = 1; //set pin 7 to analog
    
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

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

