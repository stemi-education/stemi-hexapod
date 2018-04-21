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

RobotEngine::RobotEngine(SharedData *sharedDataNew) : touch(50, 40, 5)
{
	sharedData = sharedDataNew;
}

void RobotEngine::checkState()
{
	touch.checkTouch();
	int touchState = -1;
	if (touch.isTouchDetected())
	{
		touchState = touch.getTouchPattern(true);
		Serial.print("t: ");
		Serial.println(touchState);
	}
	else
		touchState = -1;
	//make changes based on input and current robotMode
	switch (sharedData->mode)
	{
	case ROBOT_BATTERY_EMPTY_MODE:
		//rebooting the robot is the only way out of this mode
		break;
	case ROBOT_STANDBY_MODE:
		switch (touchState)
		{
		case 5:
			sharedData->mode = ROBOT_PRE_CALIBRATION_MODE;
			break;
		case 2:
			sharedData->mode = ROBOT_WALK_MODE;
			break;
		}
		break;
	case ROBOT_STANDING_UP_MODE:
		//no comands
		break;
	case ROBOT_WALK_MODE:
		switch (touchState)
		{
		case 5:
			sharedData->mode = ROBOT_PRE_CALIBRATION_MODE;
			break;
		case 2:
			sharedData->mode = ROBOT_EMPTY_MODE;
			break;
		}
		break;
	case ROBOT_EMPTY_MODE:
		switch (touchState)
		{
		case 5:
			sharedData->mode = ROBOT_PRE_CALIBRATION_MODE;
			break;
		case 2:
			sharedData->mode = ROBOT_WALK_MODE;
			break;
		}
		break;
	case ROBOT_PRE_CALIBRATION_MODE:
		switch (touchState)
		{
		case -1: //nothing is pressed - needed for default case
			break;
		case 5:
			sharedData->mode = ROBOT_CALIBRATION_MODE;
			sharedData->servoCtrl.mode = SERVO_CALIBRATION_MODE;
			break;
		default:
			sharedData->mode = ROBOT_WALK_MODE;
			break;
		}
		break;
	case ROBOT_CALIBRATION_MODE:
		switch (touchState)
		{
		case 5: //save and exit
			sharedData->mode = ROBOT_WALK_MODE;
			sharedData->servoCtrl.store = 1;
			sharedData->servoCtrl.mode = SERVO_WALKING_MODE;
			break;
		//change calibration value
		case 1:
			sharedData->servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((sharedData->servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] - 10), -100, 100);
			break;
		case 4:
			sharedData->servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((sharedData->servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] + 10), -100, 100);
			break;
		//change selected servo layer
		case 3:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			sharedData->servoCtrl.nudge = calibrationServoLayerSelected;
			break;
		case 6:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			sharedData->servoCtrl.nudge = calibrationServoLayerSelected;
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
	//Serial.println("check");
	switch (sharedData->mode)
	{
	case ROBOT_STANDBY_MODE:
		sharedData->ledCtrl.primarClr[0] = 0;
		sharedData->ledCtrl.primarClr[1] = 255;
		sharedData->ledCtrl.primarClr[2] = 255;
		sharedData->moveCtrl.linearVelocity = 0;
		sharedData->moveCtrl.angularVelocity = 0;
		sharedData->moveCtrl.poseVector[0] = 1;
		break;
	case ROBOT_STANDING_UP_MODE:
		//no comands
		break;
	case ROBOT_WALK_MODE:
		//set up walking parameters
		sharedData->ledCtrl.primarClr[0] = 0;
		sharedData->ledCtrl.primarClr[1] = 255;
		sharedData->ledCtrl.primarClr[2] = 0;
		sharedData->ledCtrl.secondarClr[0] = 0;
		sharedData->ledCtrl.secondarClr[1] = 0;
		sharedData->ledCtrl.secondarClr[2] = 0;
		sharedData->writeBtCtrlToMoveCtrl();
		sharedData->moveCtrl.poseVector[0] = 4;
		break;
	case ROBOT_EMPTY_MODE:
		//set up empty parameters
		sharedData->ledCtrl.blinkingSpeed = 0;
		sharedData->ledCtrl.rotationSpeed = 0.1;
		sharedData->ledCtrl.direction = 0;
		sharedData->ledCtrl.spreadRatio = 0.5;
		sharedData->ledCtrl.primarClr[0] = 255;
		sharedData->ledCtrl.primarClr[1] = 255;
		sharedData->ledCtrl.primarClr[2] = 255;
		sharedData->moveCtrl.linearVelocity = 5;
		sharedData->moveCtrl.angularVelocity = 0;
		sharedData->moveCtrl.poseVector[0] = 5;
		break;
	case ROBOT_PRE_CALIBRATION_MODE:
		sharedData->ledCtrl.blinkingSpeed = 0;
		sharedData->ledCtrl.rotationSpeed = 0;
		sharedData->ledCtrl.direction = 0;
		sharedData->ledCtrl.spreadRatio = 0.5;
		sharedData->ledCtrl.primarClr[0] = 100;
		sharedData->ledCtrl.primarClr[1] = 0;
		sharedData->ledCtrl.primarClr[2] = 0;
		sharedData->ledCtrl.primarClr[0] = 0;
		sharedData->ledCtrl.primarClr[1] = 0;
		sharedData->ledCtrl.primarClr[2] = 0;
		break;
	case ROBOT_CALIBRATION_MODE:
		sharedData->ledCtrl.primarClr[0] = 255;
		sharedData->ledCtrl.primarClr[1] = 0;
		sharedData->ledCtrl.primarClr[2] = 0;
		break;
	case ROBOT_BATTERY_EMPTY_MODE:
		//LEDS blinking
		sharedData->ledCtrl.primarClr[0] = 255;
		sharedData->ledCtrl.primarClr[1] = 0;
		sharedData->ledCtrl.primarClr[2] = 0;
		sharedData->moveCtrl.linearVelocity = 0;
		sharedData->moveCtrl.angularVelocity = 0;
		sharedData->servoCtrl.power = 0;
		break;
	}
}


