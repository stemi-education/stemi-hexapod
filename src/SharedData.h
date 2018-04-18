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

//led modes

//robot modes
#define ROBOT_STANDBY_MODE 0
#define ROBOT_STANDING_UP_MODE 1
#define ROBOT_WALK_MODE 2
#define ROBOT_WALk_N_TILT_MODE 3
#define ROBOT_DANCE_MODE 4
#define ROBOT_EMPTY_MODE 10 //Custom mode for users to program

#define ROBOT_PRE_CALIBRATION_MODE -1
#define ROBOT_CALIBRATION_MODE -2
#define ROBOT_BATTERY_EMPTY_MODE -3

#define LED_MANUAL_MODE 0;
#define LED_PARAMETRIC_MODE 1;
#define LED_CUSTOM_MODE 2;

class SharedData {
public:
	SharedData();
	void writeServoAngles(float servoAnglesNew[18]);
	void writeBtCtrlToMoveCtrl();

	struct MoveCtrl
	{
		float linearVelocity = 0;
		float direction = PI / 2;
		float angularVelocity = 0;
		double poseVector[6] = { 3, 0, 0, 0, 0, 0 }; //initial translation and rotation vector of roots pose
		uint timer = 0;

		//float roboHightu = 4;
		int gaitID = 1;
		boolean buttons[2] = { 0, 0 }; // up, down, full rotation, part rotation (moving) 
		float trXYu[2] = { 0, 0 }; //robot tilt
		float stepHight = 2;

		bool linMode = 0; //state, if robot is in calibration mode
		bool running = 1; //state, if the robot is ready for control (power on-off)

		int nMove = 100; //how many times will current command execute (0 = home)
		int nMoveMax = 100; //max number nMove (watchdog timer)
	} moveCtrl;

	struct BtCtrl
	{
		float linearVelocity = 0;
		float direction = PI / 2;
		float angularVelocity = 0;
		double poseVector[6] = { 3, 0, 0, 0, 0, 0 }; //initial translation and rotation vector of roots pose
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
		float Direction = PI / 2;// [0, 2pi]
		float spreadRatio = 0.5; // [0, 1] -spatial radio between primar and secondar color
		uint8_t primarClr[3] = { 0,0,255 };
		uint8_t secondarClr[3] = { 0,0,0 };
		int rotationSpeed = 0; // [-100, 100] ~degrees / sec
		uint8_t blinkingSpeed = 0; // [0=off, 10] ~blinks/sec
		uint8_t blinkShape = 0;// [0, 1, 2...](sine, square…) //TODO make MACROS with names
		uint8_t manualClr[6][3];
		int8_t mode = LED_PARAMETRIC_MODE;
	} ledCtrl;

	struct BatteryState
	{
		float voltage = 0;
		uint8_t percentage = 0;
	} batteryState;

	int8_t mode = ROBOT_STANDBY_MODE;
};



#endif
