/*

This file is part of STEMI hexapod library.

Copyright (C) 2017 STEMI d.o.o.
Authors: Luka Fucek, et al.
Contact: info@stemi.education
Web: https://www.stemi.education/

STEMI hexapod library is free software exclusively
for non-commercial activities.

In case of developing any comercial activities involving STEMI
hexapod library please contact stemi.education for providing custom
license at: info@stemi.education.

For non-commercial activities you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

STEMI hexapod library is
distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with STEMI hexapod library.
If not, see http://www.gnu.org/licenses/.

For additional information please check http://www.stemi.education.

*/


#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LED_COUNT 6
#define LED_PIN 19 //Neopixel RGB LED strip pin
#define SERVO_POWER_PIN 33

//servo modes
#define SERVO_WALKING_MODE 0 //mode for walking
#define SERVO_CALIBRATION_MODE 1 //mode for servo calibration
#define ROBOT_BATTERY_EMPTY_MODE_VOLTAGE_TRESHOLD 3.6 //before shutting down

//led modes

//robot modes
#define ROBOT_USER_MODE -1

#define ROBOT_STANDBY_MODE 0
#define ROBOT_STANDING_UP_MODE 1
#define ROBOT_WALK_MODE 2
#define ROBOT_WALK_N_TILT_MODE 3
#define ROBOT_DANCE_MODE 4
#define ROBOT_EMPTY_MODE 10 //Custom mode for users to program

#define ROBOT_PRE_CALIBRATION_MODE 21
#define ROBOT_CALIBRATION_MODE 22
#define ROBOT_BATTERY_EMPTY_MODE 23

#define LED_MANUAL_MODE 0
#define LED_PARAMETRIC_MODE 1
#define LED_CUSTOM_MODE 2

#define LED_R1 0
#define LED_R2 1
#define LED_R3 2
#define LED_L1 5
#define LED_L2 4
#define LED_L3 3

#define LED_R1_ANGLE -30
#define LED_R2_ANGLE -90
#define LED_R3_ANGLE -120
#define LED_L1_ANGLE 30
#define LED_L2_ANGLE 90
#define LED_L3_ANGLE 120

//movement parameters
#define MOVE_CTRL_TIMER_MAX 200; //iterations
#define MOVE_SPEED 5 //cm/s
#define TURN_SPEED 0.4 //radians/s
#define TILT_AMMOUNT 0.2 //radians
#define STRETCH_AMMOUNT 2 //cm

//touch parameters
#define TOUCH_000 0
#define TOUCH_X00 1
#define TOUCH_0X0 2
#define TOUCH_XX0 3
#define TOUCH_00X 4
#define TOUCH_X0X 5
#define TOUCH_0XX 6
#define TOUCH_XXX 7

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif

struct Color { uint8_t r; uint8_t g; uint8_t b; };
static Color const RED = { 255, 0, 0 };
static Color const GREEN = { 0, 255, 0 };
static Color const BLUE = { 0, 0, 255 };
static Color const YELLOW = { 255, 242, 0 };
static Color const PURPLE = { 255, 0, 255 };
static Color const WHITE = { 255, 255, 255 };
static Color const BLACK = { 0, 0, 0 };

struct MoveCtrl
{
	float linearVelocity;
	float direction;
	float angularVelocity;
	double poseVector[6]; //initial translation and rotation vector of roots pose
	int timeout; //how many times will current command execute (0 = home, -1 = infinite)
	int8_t gaitID;
	float stepHight;
};

static const MoveCtrl FORWARD =  { 5, PI / 2,			0,	{ 4,0,STRETCH_AMMOUNT,0,-TILT_AMMOUNT,0 }, -1, 1, 2 };
static const MoveCtrl BACKWARD = { 5, 3 * PI / 2, 0,	{ 4,0,-STRETCH_AMMOUNT,0,TILT_AMMOUNT,0 }, -1, 1, 2 };
static const MoveCtrl LEFT =		 { 5, PI, 0.2,				{ 4,-STRETCH_AMMOUNT,0,0,0,-TILT_AMMOUNT }, -1, 1, 2 };
static const MoveCtrl RIGHT =		 { 5, 0, -0.2,				{ 4,STRETCH_AMMOUNT,0,0,0,TILT_AMMOUNT }, -1, 1, 2 };
static const MoveCtrl RESET =		 { 0, PI/2, 0,				{ 4,0,0,0,0,0 }, -1, 1, 2 };

class SharedData {
public:
	SharedData();
	void writeServoAngles(float servoAnglesNew[18]);
	void writeBtCtrlToMoveCtrl();
	
	//LED
	void setLed(Color color);
	void setLed(Color primarClr, Color secondarClr, float spreadRatio = 2, float direction = 0);
	void setLed(uint8_t ledNo, Color color);
	void setLedRotationSpeed(float rotationSpeed);
	void setLedBlinkingSpeed(float blinkingSpeed);

	//Movement
	void move(MoveCtrl movement);
	void move(float linearVelocity, float direction, float angularVelocity);
	void rotate(MoveCtrl rotation);
	void tilt(MoveCtrl tiltation);
	void stretch(MoveCtrl stretchment);
	void setHeight(float height);

	void setMode(int8_t modeNew);
	int8_t getMode();

	void storeServoCalibrationData();

	void setServoPower(bool power);

	uint8_t getTouchPattern();
	void setTouchPattern(uint8_t touchState);
	float getBatteryVoltage();

	void enterUserMode();
	void exitUserMode();

	MoveCtrl moveCtrl;

	struct BtCtrl
	{
		float linearVelocity = 0;
		float direction = PI / 2;
		float angularVelocity = 0;
		double poseVector[6] = { 1, 0, 0, 0, 0, 0 }; //initial translation and rotation vector of roots pose
		uint timer = 0;
	} btCtrl;

	struct Parameters
	{
		double a[3] = { 1.11,4.82,6.04 }; //dimensions of one leg
		double dim[3] = { 3, 5.4, 7.2 }; //coordinates of the robots hips
		double freq = 50; //frequency of the algorithm
		double ts = 1.0 / freq;
	} param;

	struct ServoCtrl
	{
		bool power = 0;
		float servoAngles[18] = { 0, 0, 0,
															0, 0, 0,
															0, 0, 0,
															0, 0, 0,
															0, 0, 0,
															0, 0, 0 };
		bool mode = SERVO_WALKING_MODE;
		int8_t calibrationOffsetBytes[18] = {0, 0, 0, //TODO rename to calibrationPoints or similar - to be [-100,100] of general unit
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0 };
		bool store = 0;
		int8_t nudge = -1;
	} servoCtrl;

	struct LedCtrl
	{
		float direction = 0;// [0, 2pi]
		float spreadRatio = 1; // [0, 1] -spatial radio between primar and secondar color
		uint8_t primarClr[3] = { 0,0,0 };
		uint8_t secondarClr[3] = { 0,0,0 };
		float rotationSpeed = 0; // [-100, 100] ~degrees / sec
		float blinkingSpeed = 0; // [0=off, 10] ~blinks/sec
		uint8_t blinkShape = 0;// [0, 1, 2...](sine, square…) //TODO make MACROS with names
		uint8_t manualClr[6][3];
		uint8_t finalClr[6][3];
		int8_t mode = LED_PARAMETRIC_MODE;
	} ledCtrl, userLedCtrl;

	struct BatteryState
	{
		float voltage = 0;
		uint8_t percentage = 0;
	} batteryState;

	struct TouchState
	{
		uint8_t state = TOUCH_000;
	} touch;

	int8_t mode = ROBOT_USER_MODE;
};

extern SharedData robot;

#endif
