
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCKET_SERVO
#define	BUCKET_SERVO

#include <xc.h> // include processor files - each processor file is guarded.  
#include "global_variables.h"

//given angle in degrees between 0 and 180, set servo pwm
void setServoAngle(int angle){
    int width = (int)(angle * (1000.0/180.0)) + 250; //convert angle in degrees to pulse width between 250 and 1250 timer count
    servo_pulse_width = width;
}


#endif	/* XC_HEADER_TEMPLATE_H */

