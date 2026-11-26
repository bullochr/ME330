
// more than once.  
#ifndef BALL_COLLECTION
#define	BALL_COLLECTION

#include <xc.h>

#include "state.h" // include processor files - each processor file is guarded.  

void depositSample(){
    setLeftWheelVelocity(0);
    setRightWheelVelocity(0);
    waitMsBlocking(250);
    if(state.ball_white){
        turn90DegreesBlocking(0);
        state.turneddirection = 0; //note that we have turned LEFT
    }else{
        turn90DegreesBlocking(1);
        state.turneddirection = 1; //note that we have turned RIGHT
    }
    waitMsBlocking(250);
    driveDistanceBlocking(200, BASE_WHEEL_VELOCITY);   //ADJUST TO REACH BIN
    setServoAngle(MIN_SERVO_ANGLE);
    waitMsBlocking(1000);
    setServoAngle(40);
    waitMsBlocking(1000);
    setLeftWheelVelocity(-SLOW_WHEEL_VELOCITY);
    setRightWheelVelocity(-SLOW_WHEEL_VELOCITY);
}

#endif	/* XC_HEADER_TEMPLATE_H */

