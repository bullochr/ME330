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
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void configure_pins(void){
        //Configure pins
    OC1CON1bits.OCTSEL = 0b111; //set PWM on pin 14 (left wheel) to use internal system clock
    OC2CON1bits.OCTSEL = 0b111; //set PWM on pin 4 (right wheel) to use internal system clock
    
    OC1CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 14
    OC2CON1bits.OCM = 0b000; //initially DISABLE PWM on pin 4
    
    OC1CON2bits.SYNCSEL = 0b11111; //configure PWM module sync source on pin 14
    OC2CON2bits.SYNCSEL = 0b11111; //configure PWM module sync source on pin 4
    
    OC1RS = 4000; //set the period of PWM on PIN 14 in clock cycles
    OC2RS = 4000; //set the period of PWM on PIN 4 in clock cycles
    
    OC1R = 1999; //50% Duty cycle on pin 14
    OC2R = 1999; //50% Duty cycle on pin 4
    
    _OC1IP = 4; //set pin 14 interrupt priority to 4
    _OC2IP = 4; //set pin 4 interrupt priority to 4
    
    _OC1IE = 0; //enable interrupt on pin 14
    _OC2IE = 0; //enable interrupt on pin 4
    
    _OC1IF = 1; //clear flag on pin 14 
    _OC2IF = 1; //clear flag on pin 4 
    
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

