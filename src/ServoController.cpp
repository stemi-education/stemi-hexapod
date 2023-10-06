#include "ServoController.h"
#include "Semaphore.h"

ServoController::ServoController() : servo(115, 302, 519)
{
  
	pwmPCA9685.resetDevices();
	pwmPCA9685.init(B000000);
	pwmPCA9685.setPWMFrequency(50);

	ledcSetup(1, 50, 16);
	ledcSetup(2, 50, 16);
	ledcSetup(3, 50, 16);
	ledcAttachPin(18, 1);
	ledcAttachPin(21, 2);
	ledcAttachPin(EXTRA_SERVO_PIN, 3);

}

void ServoController::moveAllServos(float radianPositions[18], float extraServoPosition)
{
  
    // take care of phyisical servo orientation, convert radian to degrees and
    // then convert it to PWM duratio
  uint16_t millisPCA9685[16];
	uint16_t pwmESP32[3];

	//millisPCA9685
	millisPCA9685[0] = servo.pwmForAngle(mapSaturate(radianPositions[6] * (180 / PI), 90, -90, -90, 90, -80, 80));//R31
	millisPCA9685[1] = servo.pwmForAngle(mapSaturate(radianPositions[7] * (180 / PI), 90, -90, -90, 90, -80, 80));//R32
	millisPCA9685[2] = servo.pwmForAngle(mapSaturate(radianPositions[3] * (180 / PI), 90, -90, -90, 90, -80, 80));//R21

	millisPCA9685[3] = servo.pwmForAngle(mapSaturate(radianPositions[4] * (180 / PI), 90, -90, -90, 90, -80, 80));//R22
	millisPCA9685[4] = servo.pwmForAngle(mapSaturate(radianPositions[5] * (180 / PI), -180, 0, -90, 90, -80, 80));//R23
	millisPCA9685[5] = servo.pwmForAngle(mapSaturate(radianPositions[0] * (180 / PI), 90, -90, -90, 90, -80, 80));//R11

	millisPCA9685[6] = servo.pwmForAngle(mapSaturate(radianPositions[1] * (180 / PI), 90, -90, -90, 90, -80, 80));//R12
	millisPCA9685[7] = servo.pwmForAngle(mapSaturate(radianPositions[2] * (180 / PI), -180, 0, -90, 90, -80, 80));//R13
	millisPCA9685[8] = servo.pwmForAngle(mapSaturate(radianPositions[11] * (180 / PI), -180, 0, 90, -90, -80, 80));//L13

	millisPCA9685[9] = servo.pwmForAngle(mapSaturate(radianPositions[10] * (180 / PI), 90, -90, 90, -90, -80, 80));//L12
	millisPCA9685[10] = servo.pwmForAngle(mapSaturate(radianPositions[9] * (180 / PI), 90, -90, -90, 90, -80, 80));//L11
	millisPCA9685[11] = servo.pwmForAngle(mapSaturate(radianPositions[14] * (180 / PI), -180, 0, 90, -90, -80, 80));//L23

	millisPCA9685[12] = servo.pwmForAngle(mapSaturate(radianPositions[13] * (180 / PI), 90, -90, 90, -90, -80, 80));//L22
	millisPCA9685[13] = servo.pwmForAngle(mapSaturate(radianPositions[12] * (180 / PI), 90, -90, -90, 90, -80, 80));//L21
	millisPCA9685[14] = servo.pwmForAngle(mapSaturate(radianPositions[15] * (180 / PI), 90, -90, -90, 90, -80, 80));//L31
	
	millisPCA9685[15] = servo.pwmForAngle(mapSaturate(radianPositions[8] * (180 / PI), -180, 0, -90, 90, -80, 80));//R33

	//millisM0
	pwmESP32[0] = mapSaturate(radianPositions[16] * (180 / PI), 90, -90, 7630, 1700, 2029, 7300);//L32
	pwmESP32[1] = mapSaturate(radianPositions[17] * (180 / PI), -180, 0, 7630, 1700, 2029, 7300);//L33
	pwmESP32[2] = mapSaturate(extraServoPosition, 90, -90, 7630, 1700, 2029, 7300);//EXTRA SERVO

	takeSemaphore();
	pwmPCA9685.setChannelsPWM(0, 16, millisPCA9685);
	ledcWrite(1, pwmESP32[0]);
	ledcWrite(2, pwmESP32[1]);
	ledcWrite(3, pwmESP32[2]);
	giveSemaphore();
}


