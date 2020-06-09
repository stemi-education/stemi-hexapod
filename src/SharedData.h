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

#include "Names.h"

#define HEXAPOD_SW_VERSION_MAJOR 2
#define HEXAPOD_SW_VERSION_MINOR 0
#define HEXAPOD_SW_VERSION_PATCH 10
#define HEXAPOD_HW_VERSION_MAJOR 2
#define HEXAPOD_HW_VERSION_MINOR 0
#define HEXAPOD_HW_VERSION_PATCH 0

#define ENABLE_VERSION_BURNING
//#define DEBUG 
//#define DEBUG_VOLTAGES

#define LED_COUNT 6
#define LED_PIN 19 //Neopixel RGB LED strip pin
#define SERVO_POWER_PIN 33
#define EXTRA_SERVO_PIN 25

//task frequencies
#define TASK_PERIOD_WALK 20
#define TASK_PERIOD_LED 10
#define TASK_PERIOD_BT 200
#define TASK_PERIOD_TOUCH 20
#define TASK_PERIOD_SERVO 20
#define TASK_PERIOD_ROBOT 20
#define TASK_PERIOD_BATT 200
#define TASK_PERIOD_DANCE 275 //half of a songs beat

//movement duration
#define MOVE_DURATION_DEFAULT -1 //infinite

//servo modes
#define SERVO_WALKING_MODE 0 //mode for walking
#define SERVO_CALIBRATION_MODE 1 //mode for servo calibration
#define ROBOT_BATTERY_EMPTY_MODE_VOLTAGE_TRESHOLD 3.6 //before shutting down

//robot modes
#define ROBOT_USER_MODE -1

#define ROBOT_STANDBY_MODE 0
#define ROBOT_WALK_MODE 1
#define ROBOT_DANCE_MODE 2

#define ROBOT_SETUP_MODE 10
#define ROBOT_CALIBRATION_MODE 11
#define ROBOT_BATTERY_EMPTY_MODE 12

//led modes
#define LED_MANUAL_MODE 0
#define LED_PARAMETRIC_MODE 1
#define LED_CUSTOM_MODE 2

//battery states
#define BATTERY_EMPTY_STATE 0
#define BATTERY_LOW_STATE 1
#define BATTERY_MID_STATE 2
#define BATTERY_HIGH_STATE 3

#define MIN_TURN_ON_VOLTAGE 3.65

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
#define MOVE_SPEED 30 //%
#define TURN_SPEED 50 //%
#define TILT_AMMOUNT 70 //%
#define STRETCH_AMMOUNT 70 //%
#define STRETCH_MAX 3 //radians
#define TILT_MAX 0.3 //radians

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

//color constants
struct Color { uint8_t r; uint8_t g; uint8_t b; };
static Color const RED = { 255, 0, 0 };
static Color const GREEN = { 0, 255, 0 };
static Color const BLUE = { 0, 0, 255 };
static Color const YELLOW = { 255, 242, 0 };
static Color const PURPLE = { 255, 0, 255 };
static Color const CYAN = { 0, 255, 255 };
static Color const WHITE = { 255, 255, 255 };
static Color const ORANGE = { 255, 60, 0 };
static Color const BLACK = { 0, 0, 0 };

struct userPresetInputData
{
	uint8_t linearVelocity; // [0,100]%
	int16_t direction; // [-180,180]degree
	int8_t  angularVelocity; // [-100,100]%
	int8_t translationX; // [-100,100]%
	int8_t translationY; // [-100,100]%
	int8_t translationZ; // [-100,100]%
	int8_t rotationX; // [-100,100]%
	int8_t rotationY; // [-100,100]%
	int8_t rotationZ; // [-100,100]%
};

//movement constants
static const userPresetInputData FORWARD = { MOVE_SPEED, 0,0   ,0,STRETCH_AMMOUNT,0,   -TILT_AMMOUNT,0,0 };
static const userPresetInputData BACKWARD = { MOVE_SPEED, 180,0   ,0,-STRETCH_AMMOUNT,0,   TILT_AMMOUNT,0,0 };
static const userPresetInputData LEFT = { MOVE_SPEED, 90, MOVE_SPEED   ,-STRETCH_AMMOUNT,0,0,   0,-TILT_AMMOUNT,0 };
static const userPresetInputData RIGHT = { MOVE_SPEED, -90, -MOVE_SPEED   ,STRETCH_AMMOUNT,0,0,   0,TILT_AMMOUNT,0 };
static const userPresetInputData STOP = { 0,0,0, 0,0,0, 0,0,0 };

struct MovementData
{
	uint8_t linearVelocity; // [0,100]%
	int16_t direction; // [-180,180]degree
	int8_t  angularVelocity; // [-100,100]%
};

struct PoseData
{
	int8_t translationX; // [-100,100]%
	int8_t translationY; // [-100,100]%
	int8_t translationZ; // [-100,100]%
	int8_t rotationX; // [-100,100]%
	int8_t rotationY; // [-100,100]%
	int8_t rotationZ; // [-100,100]%
	int8_t poseSpeed; // [0,100]% - temporary [99,30]
};

struct LedData
{
	uint8_t ledPrimarClr[3]; // [255]r, [255]g, [255]b
	uint8_t ledSecondarClr[3]; // [255]r, [255]g, [255]b
	int16_t ledDiretion; // [-180,180]degree
	uint8_t ledSpreadRatio; // [0,100]%
	int8_t ledRotationSpeed; // [-100,100]%
	uint8_t ledBlinkingSpeed; // [0,100]%
	uint8_t ledManualClr[6][3]; // 6x [255]r, [255]g, [255]b
	int8_t ledMode; // [manual, parametric]mode
};

struct InputData : MovementData, PoseData, LedData
{
	//Movement - inherited from MovementData

	//Pose - inherited from PoseData
	
	//Led - inherited from LedData

	//Misc - additional data
	//int8_t robotMode; //[check ROBOT_XX_MODE macros]mode
	int16_t moveDuration = 0; // miliseconds of current command execution [-1 = inf, 0 = go home, ...]
	int8_t gaitID = 3; //[0,5]gait
	uint8_t stepHeight = 50; // [0,100]%
	bool servoPower = 1; // [on,off]power
	int16_t extraServoAngle = 0;
};

struct PhisicsAndMoveParameters
{
	double a[3] = { 1.11,4.82,6.04 }; //dimensions of one leg
	double dim[3] = { 3, 5.4, 7.2 }; //coordinates of the robots hips
	double freq = 50; //frequency of the algorithm
	double ts = 1.0 / freq;
	int8_t gaitID = 3;
	float stepHeight = 2;
	float poseChangeSpeed = 0.93; //parameter for PT1 filter recomended [0.90,0.98]
};

struct MoveCtrl
{
	float linearVelocity;
	float direction;
	float angularVelocity;
	double poseVector[6]; //initial translation and rotation vector of roots pose
	int timeout; //how many times will current command execute (0 = home, -1 = infinite)
};

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
	int8_t calibrationOffsetBytes[18] = { 0, 0, 0, //TODO rename to calibrationPoints or similar - to be [-100,100] of general unit
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0,
																				0, 0, 0 };
	bool store = 0;
	int8_t nudge = -1;
	float extraServoAngle = 90;
};

struct LedCtrl
{
	float direction = 0;// [0, 2pi]
	float spreadRatio = 1; // [0, 1] -spatial radio between primar and secondar color
	uint8_t primarClr[3] = { 0,0,0 };
	uint8_t secondarClr[3] = { 0,0,0 };
	float rotationSpeed = 0; // [-10, 0=off, 10] ~ rotations / s
	float blinkingSpeed = 0; // [0=off, 10] ~ blinks / s
	uint8_t blinkShape = 0;// [0, 1, 2...](sine, square�) //TODO make MACROS with names
	uint8_t manualClr[6][3];
	uint8_t finalClr[6][3];
	int8_t mode = LED_PARAMETRIC_MODE;
};

struct BatteryState
{
	float voltage = 0;
	uint8_t percentage = 50;
	uint8_t state = BATTERY_MID_STATE;
	bool store = 0;
};

struct TouchState
{
	uint8_t state = TOUCH_000;
};

class SharedData {
public:
	SharedData();
	
	//LED
	void setLed(Color color);
	void _setLed(Color color);
	void setLed(Color primarClr, Color secondarClr, uint8_t spreadRatio, int16_t direction);
	void _setLed(Color primarClr, Color secondarClr, float spreadRatio, float direction);
	void setLedRotationSpeed(int8_t rotationSpeed);
	void _setLedRotationSpeed(float rotationSpeed);
	void setLedBlinkingSpeed(uint8_t blinkingSpeed);
	void _setLedBlinkingSpeed(float blinkingSpeed);
	void setLedStatic(uint8_t ledNo, Color color);
	void setLedStatic(Color color);
	void _setLedStatic(uint8_t ledNo, Color color);
	void _setLedStatic(Color color);

	//Movement
	void move(userPresetInputData movement, float duration = MOVE_DURATION_DEFAULT);
	void move(uint8_t linearVelocity, int16_t direction, int8_t angularVelocity, float duration = MOVE_DURATION_DEFAULT);
	void rotate(userPresetInputData rotation, float duration = MOVE_DURATION_DEFAULT);
	void _move(float linearVelocity, float direction, float angularVelocity, float duration = MOVE_DURATION_DEFAULT);
	void tilt(userPresetInputData tiltation);
	void tilt(int8_t rotationXNew, int8_t rotationYNew, int8_t rotationZNew);
	void stretch(userPresetInputData stretchment);
	void stretch(int8_t translationXNew, int8_t treanslationYNew);
	void setHeight(uint8_t height);
	void _setHeight(float height);
	void _setPose(float poseVectorNew[6]);

	//Servo functions
	void writeServoAngles(float servoAnglesNew[18]);
	void storeServoCalibrationData();
	void setServoPower(bool power);
	void _setServoPower(bool power);

	//Extra servo functions
	void writeExtraServo(float servoAngle);
	void _writeExtraServo(float servoAngle);

	//touch functions
	uint8_t getTouchPattern();
	uint8_t _getTouchPattern();
	void _setTouchPattern(uint8_t touchState);

	//Battery functions
	float getBatteryVoltage();
	uint8_t getBatteryPercentage();

	//Robot modes functions
	void setMode(int8_t modeNew);
	int8_t getMode();
	void enterUserMode();
	void exitUserMode();

	//Naming the robot
	void loadName();
	void storeName(std::string nameNew);

	//choose what input data to use [BT, user, dance]
	void useGeneralInputData(InputData * data);
	void useMoveInputData(InputData * data);
	void useLedInputData(InputData * data);

	//UniversalData to be written from bluetooth or other sources
	int8_t universalData[4] = { 0, 0, 0, 0, };

	MoveCtrl moveCtrl;
	InputData btInputData, userInputData, danceInputData;
	PhisicsAndMoveParameters PMParam;
	ServoCtrl servoCtrl;
	LedCtrl ledCtrl;
	BatteryState battery;
	TouchState touch;

	//Robot name variables
	Names names;

	int8_t mode = ROBOT_STANDBY_MODE;
	uint8_t BTConnectedCount = 0;
	int8_t userSlider = 0;
	uint8_t hexSwVersion[3] = { HEXAPOD_SW_VERSION_MAJOR, HEXAPOD_SW_VERSION_MINOR, HEXAPOD_SW_VERSION_PATCH };
	uint8_t hexHwVersion[3] = { HEXAPOD_HW_VERSION_MAJOR, HEXAPOD_HW_VERSION_MINOR, HEXAPOD_HW_VERSION_PATCH };
	uint danceCount = 0;
	std::string name;
};

// general functions:
static float saturate(float value, float minimum, float maximum)
{
	return min(maximum, max(minimum, value));
}

static float mapSaturate(float x, float in_min, float in_max, float out_min, float out_max, float sat_min, float sat_max)
{
	return saturate((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, sat_min, sat_max);
}
static float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
extern SharedData robot;
#endif
