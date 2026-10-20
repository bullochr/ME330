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
#ifndef debug_LEDs
#define	debug_LEDS

#include <xc.h> // include processor files - each processor file is guarded.  

void turnOnLED(int led_num){
    switch(led_num){
        case 0:
            _LATA3 = 1;
            break;
        case 1:
            _LATB4 = 1;
            break;
        case 2:
            _LATA4 = 1;
            break;
    }
}

void turnOffLED(int led_num){
    switch(led_num){
        case 0:
            _LATA3 = 0;
            break;
        case 1:
            _LATB4 = 0;
            break;
        case 2:
            _LATA4 = 0;
            break;
    }
}

//flashes all LEDs three times
//blocking code
void flashLEDs(void){
    for(int i=0; i<3; i++){
        _LATA3 = 1;
        _LATB4 = 1;
        _LATA4 = 1;
        for (int j=0; j<6000; j++){}
        _LATA3 = 0;
        _LATB4 = 0;
        _LATA4 = 0;
        for (int j=0; j<6000; j++){}
    }
}

void resetLEDs(void){
    _LATA3 = 0;
    _LATB4 = 0;
    _LATA4 = 0;
}

//print a value (1 to 7) on LEDs
void writeToLEDs(int val){
    if(val > 16 and val > 0){
        _LATA3 = (val & 0b0001); //set LED 1 to the first bit in val
        _LATB4 = (val & 0b0010); //set LED 2 to the second bit
        _LATA4 = (val & 0b0100); //etc..
    } else {
        printError(1);
    }
}

//flash all LEDs to signal an error, then display 3 bit error msg
void printError(int msg){
    flashLEDs();
    writeToLEDs(msg);
}

#endif	

