#include "ServoController.h"

ServoController::ServoController() : servo(125, 330, 551)
{
  Wire.begin(23,22);
  
	pwmPCA9685.resetDevices();
	pwmPCA9685.init(B000000);
	pwmPCA9685.setPWMFrequency(50);

	ledcSetup(1, 50, 16);
	ledcSetup(2, 50, 16);
	ledcAttachPin(18, 1);
	ledcAttachPin(21, 2);

}

void ServoController::moveAllServos(float radianPositions[18])
{
  
    // take care of phyisical servo orientation, convert radian to degrees and
    // then convert it to PWM duratio
  uint16_t millisPCA9685[16];
	uint16_t pwmESP32[2];

	//millisPCA9685
	millisPCA9685[0] = servo.pwmForAngle(mapf(radianPositions[6] * (180 / PI), 90, -90, -90, 90));//R31
	millisPCA9685[1] = servo.pwmForAngle(mapf(radianPositions[7] * (180 / PI), 90, -90, -90, 90));//R32
	millisPCA9685[2] = servo.pwmForAngle(mapf(radianPositions[3] * (180 / PI), 90, -90, -90, 90));//R21

	millisPCA9685[3] = servo.pwmForAngle(mapf(radianPositions[4] * (180 / PI), 90, -90, -90, 90));//R22
	millisPCA9685[4] = servo.pwmForAngle(mapf(radianPositions[5] * (180 / PI), -180, 0, -90, 90));//R23
	millisPCA9685[5] = servo.pwmForAngle(mapf(radianPositions[0] * (180 / PI), 90, -90, -90, 90));//R11

	millisPCA9685[6] = servo.pwmForAngle(mapf(radianPositions[1] * (180 / PI), 90, -90, -90, 90));//R12
	millisPCA9685[7] = servo.pwmForAngle(mapf(radianPositions[2] * (180 / PI), -180, 0, -90, 90));//R13
	millisPCA9685[8] = servo.pwmForAngle(mapf(radianPositions[11] * (180 / PI), -180, 0, 90, -90));//L13

	millisPCA9685[9] = servo.pwmForAngle(mapf(radianPositions[10] * (180 / PI), 90, -90, 90, -90));//L12
	millisPCA9685[10] = servo.pwmForAngle(mapf(radianPositions[9] * (180 / PI), 90, -90, -90, 90));//L11
	millisPCA9685[11] = servo.pwmForAngle(mapf(radianPositions[14] * (180 / PI), -180, 0, 90, -90));//L23

	millisPCA9685[12] = servo.pwmForAngle(mapf(radianPositions[13] * (180 / PI), 90, -90, 90, -90));//L22
	millisPCA9685[13] = servo.pwmForAngle(mapf(radianPositions[12] * (180 / PI), 90, -90, -90, 90));//L21
	millisPCA9685[14] = servo.pwmForAngle(mapf(radianPositions[15] * (180 / PI), 90, -90, -90, 90));//L31
	
	millisPCA9685[15] = servo.pwmForAngle(mapf(radianPositions[8] * (180 / PI), -180, 0, -90, 90));//R33

	//millisM0
	pwmESP32[0] = mapf(radianPositions[16] * (180 / PI), 90, -90, 8000, 1700);//L32
	pwmESP32[1] = mapf(radianPositions[17] * (180 / PI), -180, 0, 8000, 1700);//L33

	pwmPCA9685.setChannelsPWM(0, 16, millisPCA9685);
	ledcWrite(1, pwmESP32[0]);
	ledcWrite(2, pwmESP32[1]);
}

float ServoController::mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
