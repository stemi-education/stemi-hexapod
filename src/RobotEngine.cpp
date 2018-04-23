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


#include "RobotEngine.h"

float saturate(float value, float minimum, float maximum)
{
	return min(maximum, max(minimum, value));
}

RobotEngine::RobotEngine() : touch(50, 40, 5)
{
}

void RobotEngine::checkState()
{
	int touchState = -1;
	if (robot.getMode() != ROBOT_USER_MODE) // if in user mode do nothing
	{
		batteryCheck();
		touch.checkTouch();
		
		if (touch.isTouchDetected())
		{
			touchState = touch.getTouchPattern(true);
			Serial.print("t: ");
			Serial.println(touchState);
		}
		else
			touchState = -1;

	}
	//make changes based on input and current robotMode
	switch (robot.getMode())
	{
	case ROBOT_USER_MODE:
		//do nothing
		break;
	case ROBOT_BATTERY_EMPTY_MODE:
		//rebooting the robot is the only way out of this mode
		break;
	case ROBOT_STANDBY_MODE:
		switch (touchState)
		{
		case 5:
			robot.setMode(ROBOT_PRE_CALIBRATION_MODE);
			break;
		case 2:
			robot.setMode(ROBOT_WALK_MODE);
			break;
		}
		break;
	case ROBOT_WALK_MODE:
		switch (touchState)
		{
		case 5:
			robot.setMode(ROBOT_PRE_CALIBRATION_MODE);
			break;
		case 2:
			robot.setMode(ROBOT_WALK_N_TILT_MODE);
			break;
		}
		break;
	case ROBOT_WALK_N_TILT_MODE:
		switch (touchState)
		{
		case 5:
			robot.setMode(ROBOT_PRE_CALIBRATION_MODE);
			break;
		case 2:
			robot.setMode(ROBOT_DANCE_MODE);
			break;
		}
		break;
	case ROBOT_DANCE_MODE:
		switch (touchState)
		{
		case 5:
			robot.setMode(ROBOT_PRE_CALIBRATION_MODE);
			break;
		case 2:
			robot.setMode(ROBOT_EMPTY_MODE);
			break;
		}
		break;
	case ROBOT_EMPTY_MODE:
		switch (touchState)
		{
		case 5:
			robot.setMode(ROBOT_PRE_CALIBRATION_MODE);
			break;
		case 2:
			robot.setMode(ROBOT_WALK_MODE);
			break;
		}
		break;
	case ROBOT_PRE_CALIBRATION_MODE:
		switch (touchState)
		{
		case -1: //nothing is pressed - needed for default case
			break;
		case 5:
			robot.setMode(ROBOT_CALIBRATION_MODE);
			robot.servoCtrl.mode = SERVO_CALIBRATION_MODE;
			break;
		default:
			robot.setMode(ROBOT_WALK_MODE);
			break;
		}
		break;
	case ROBOT_CALIBRATION_MODE:
		switch (touchState)
		{
		case 5: //save and exit
			robot.setMode(ROBOT_WALK_MODE);
			robot.storeServoCalibrationData();
			robot.servoCtrl.mode = SERVO_WALKING_MODE;
			break;
		//change calibration value
		case 1:
			robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] - 10), -100, 100);
			break;
		case 4:
			robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] + 10), -100, 100);
			break;
		//change selected servo layer
		case 3:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			robot.servoCtrl.nudge = calibrationServoLayerSelected;
			break;
		case 6:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			robot.servoCtrl.nudge = calibrationServoLayerSelected;
			break;
		//change selected leg
		case 2:
			calibrationLegSelectedCounter = (calibrationLegSelectedCounter + 1) % 6;
			calibrationLegSelected = calibrationLegSelectedMap[calibrationLegSelectedCounter];
			Serial.println(calibrationLegSelected);
			break;
		}
		break;
	}
}

void RobotEngine::modesGO()
{
	uint8_t randomDummy = random(0, 255);
	Color randomColor = { randomDummy, 255 - randomDummy, randomDummy/2};

	//Serial.println("check");
	switch (robot.getMode())
	{
	case ROBOT_USER_MODE:
		//do nothing
		break;
	case ROBOT_STANDBY_MODE:
		robot.setLed(BLUE, GREEN, 2, 0);
		robot.setLedBlinkingSpeed(0);
		robot.setLedRotationSpeed(0.5);
		robot.move(RESET);
		robot.setHeight(1);
		break;
	case ROBOT_WALK_MODE:
		//set up walking parameters
		robot.setLedBlinkingSpeed(0.5);
		robot.setLedRotationSpeed(0);
		robot.setLed(BLUE, RED, 2, 0);
		robot.setHeight(4);
		robot.writeBtCtrlToMoveCtrl();
		break;
	case ROBOT_WALK_N_TILT_MODE:

		robot.setMode(ROBOT_WALK_N_TILT_MODE);

		//set up walking parameters
		robot.setLedBlinkingSpeed(0);
		robot.setLedRotationSpeed(1);
		robot.setLed(YELLOW, BLUE, 2, 0);
		robot.writeBtCtrlToMoveCtrl();
		robot.setHeight(4);
		break;
	case ROBOT_DANCE_MODE:
		robot.setLedBlinkingSpeed(2);
		robot.setLedRotationSpeed(2);
		robot.setLed(randomColor);
		robot.setHeight(4);
		break;
	case ROBOT_EMPTY_MODE:
		//set up empty parameters
		robot.setLedBlinkingSpeed(0);
		robot.setLedRotationSpeed(0);
		robot.setLed(WHITE);
		robot.setHeight(4);
		break;
	case ROBOT_PRE_CALIBRATION_MODE:
		robot.setLedBlinkingSpeed(0);
		robot.setLedRotationSpeed(0);
		robot.setLed(RED);
		break;
	case ROBOT_CALIBRATION_MODE:
		robot.setLedBlinkingSpeed(0);
		robot.setLedRotationSpeed(0);

		robot.setLed(RED, BLACK, 0.2, 0);
		break;
	case ROBOT_BATTERY_EMPTY_MODE:
		//LEDS blinking
		robot.setLedBlinkingSpeed(0.5);
		robot.setLedRotationSpeed(0);

		robot.setLed(RED);
		//turn off the servos
		robot.setServoPower(0);
		break;
	}
}

void RobotEngine::batteryCheck()
{
	//Serial.println(robot.batteryState.voltage);
	if (robot.batteryState.voltage < ROBOT_BATTERY_EMPTY_MODE_VOLTAGE_TRESHOLD)
	{
		//shut down servos and put to battery empty mode
		robot.setServoPower(0);
		robot.setMode(ROBOT_BATTERY_EMPTY_MODE);
	}
}


