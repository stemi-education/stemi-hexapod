#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include "PCA9685.h"
#include "Wire.h"
#include "SharedData.h"


//removing this comment will eneable additional prints for debug
//please be advised that this will drasticly slow down the process
//#define SERVO_DEBUG 1

class ServoController
{
public:
  ServoController();

  // radianPositions should be ordered like
  // [ r11,r12,r13,r21,r22,r23,r31,r32,r33, 
  //   l11,l12,l13,l21,l22,l23,l31,l32,l33 ]
  void moveAllServos(float radianPositions[18]);

private:
  PCA9685 pwmPCA9685;
  PCA9685_ServoEvaluator servo;
};

#endif
