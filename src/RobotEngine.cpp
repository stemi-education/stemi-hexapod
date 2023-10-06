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

RobotEngine::RobotEngine()
{
}

void RobotEngine::checkState()
{
	int touchState = 0;
	if (robot.getMode() != ROBOT_USER_MODE) // if in user mode do nothing
	{
		//batteryCheck();
		touchState = robot._getTouchPattern();
	}
	//make changes based on input and current robotMode
	int8_t robotMode = robot.getMode();
	switch (robotMode)
	{
	case ROBOT_USER_MODE:
		//apply user data
		break;
	case ROBOT_BATTERY_EMPTY_MODE:
		//rebooting the robot is the only way out of this mode
		switch (touchState)
		{
		case TOUCH_0XX:
			calibrateBattery(touchState, ROBOT_BATTERY_EMPTY_MODE);
			break;
		case TOUCH_XX0:
			calibrateBattery(touchState, ROBOT_BATTERY_EMPTY_MODE);
			break;
		}
		break;
	case ROBOT_STANDBY_MODE:
		switch (touchState)
		{
		case TOUCH_X0X:
			robot.setMode(ROBOT_SETUP_MODE);
			break;
		case TOUCH_0X0:
			robot.setMode(ROBOT_WALK_MODE);
			break;
		}
		break;
	case ROBOT_WALK_MODE:
		switch (touchState)
		{
		case TOUCH_X0X:
			robot.setMode(ROBOT_SETUP_MODE);
			break;
		case TOUCH_0X0:
			robot.setMode(ROBOT_DANCE_MODE);
			break;
		case TOUCH_00X:
			if (!robot.BTConnectedCount && !robot.isSerialConnectionOn)
				robot._move(3, PI / 2, 0, 2000);
			break;
		case TOUCH_X00:
			if (!robot.BTConnectedCount && !robot.isSerialConnectionOn)
				robot._move(3, -PI / 2, 0, 2000);
			break;
		case TOUCH_0XX:
			if (!robot.BTConnectedCount && !robot.isSerialConnectionOn)
				robot._writeExtraServo(-80);
			break;
		case TOUCH_XX0:
			if (!robot.BTConnectedCount && !robot.isSerialConnectionOn)
				robot._writeExtraServo(80);
			break;
		}
		break;
	case ROBOT_DANCE_MODE:
		switch (touchState)
		{
		case TOUCH_X0X:
			robot.setMode(ROBOT_SETUP_MODE);
			break;
		case TOUCH_0X0:
			robot.setMode(ROBOT_USER_MODE);
			break;
		}
		break;
	case ROBOT_SETUP_MODE:
		switch (touchState)
		{
		case TOUCH_000: //nothing is pressed - needed for default case
			break;
		case TOUCH_X0X:
			robot.setMode(ROBOT_CALIBRATION_MODE);
			robot.servoCtrl.mode = SERVO_CALIBRATION_MODE;
			batteryCalibrationTouchPasswordCounter = 0; //reset the counter for battery calibration password
			break;
		case TOUCH_0XX:
			calibrateBattery(touchState,ROBOT_WALK_MODE);
			break;
		case TOUCH_XX0:
			calibrateBattery(touchState, ROBOT_WALK_MODE);
			break;
		default:
			robot.setMode(ROBOT_WALK_MODE);
			batteryCalibrationTouchPasswordCounter = 0; //reset the counter for battery calibration password
			break;
		}
		break;
	case ROBOT_CALIBRATION_MODE:
		switch (touchState)
		{
		case TOUCH_X0X: //save and exit
			robot.setMode(ROBOT_WALK_MODE);
			robot.storeServoCalibrationData();
			robot.servoCtrl.mode = SERVO_WALKING_MODE;
			break;
		//change calibration value
		case TOUCH_X00:
			robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] - 10), -100, 100);
			break;
		case TOUCH_00X:
			robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] =
				saturate((robot.servoCtrl.calibrationOffsetBytes[calibrationLegSelected * 3 + calibrationServoLayerSelected] + 10), -100, 100);
			break;
		//change selected servo layer
		case TOUCH_XX0:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			robot.servoCtrl.nudge = calibrationServoLayerSelected;
			break;
		case TOUCH_0XX:
			calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
			robot.servoCtrl.nudge = calibrationServoLayerSelected;
			break;
		//change selected leg
		case TOUCH_0X0:
			calibrationLegSelectedCounter = (calibrationLegSelectedCounter + 1) % 6;
			calibrationLegSelected = calibrationLegSelectedMap[calibrationLegSelectedCounter];
			Serial.println(calibrationLegSelected);
			break;
		case TOUCH_XXX:
			for (int i = 0; i < 18; i++)
				robot.servoCtrl.calibrationOffsetBytes[i] = 0;
			break;
		}
		break;
	}
}

void RobotEngine::modesGO()
{
	int8_t robotMode = robot.getMode();
	switch (robotMode)
	{
	case ROBOT_STANDBY_MODE:
		if (robot.BTConnectedCount || robot.isSerialConnectionOn)
		{
			if (robot.ledCtrl.mode != LED_MANUAL_MODE) {
				robot.useLedInputData(&robot.btInputData);
			}
		}
		else
		{
			robot._setLed(BLUE, BLACK, 2, 1);
			robot._setLedBlinkingSpeed(0);
			robot._setLedRotationSpeed(1);
		}
		robot._move(0,0,0);
		robot._setHeight(1);
		break;

	case ROBOT_WALK_MODE:
		//set up walking parameters
		if (robot.BTConnectedCount || robot.isSerialConnectionOn)
		{
			robot.useMoveInputData(&robot.btInputData);
			if (robot.ledCtrl.mode != LED_MANUAL_MODE) {
				robot.useLedInputData(&robot.btInputData);
			}
			robot._writeExtraServo(robot.universalData[0]);
		}
		else
		{
			robot._setLed(BLUE, WHITE, 6, PI / 2);
			robot._setLedBlinkingSpeed(0);
			robot._setLedRotationSpeed(1);
			robot._setHeight(4.5);
		}
		break;

	case ROBOT_DANCE_MODE:
		robot._setLedBlinkingSpeed(0.5);
		robot._setLedRotationSpeed(0.5);
		robot._setLed(BLUE, GREEN, 2, 1);
		robot.useMoveInputData(&robot.danceInputData);
		robot.useLedInputData(&robot.danceInputData);
		break;

	case ROBOT_USER_MODE:
		robot.useMoveInputData(&robot.userInputData);
		robot.useLedInputData(&robot.userInputData);
		break;

	case ROBOT_SETUP_MODE:
		robot._setLedBlinkingSpeed(0);
		robot._setLedRotationSpeed(0);
		robot._setLed(RED);
		break;

	case ROBOT_CALIBRATION_MODE:
		robot._setLedBlinkingSpeed(0);
		robot._setLedRotationSpeed(0);
		robot._setLedStatic(BLACK);
		robot._setLedStatic(calibrationLegSelectedCounter, calibrationServoLayerColors[calibrationServoLayerSelected]);// calibrationServoLayerColors[calibrationServoLayerSelected]);
		break;

	case ROBOT_BATTERY_EMPTY_MODE:
		//LEDS blinking
		robot._setLedBlinkingSpeed(0.5);
		robot._setLedRotationSpeed(0);

		robot._setLed(RED);
		//turn off the servos
		robot._setServoPower(0);
		break;
	}
}

void RobotEngine::calibrateBattery(uint8_t touchID, int8_t exitMode)
{
	// Calibration removed
	return;
#ifdef DEBUG
	Serial.print("t: ");
	Serial.print(touchID);
	Serial.print(" key: ");
	Serial.print(batteryCalibrationTouchPassword[batteryCalibrationTouchPasswordCounter]);
	Serial.print(" c: ");
#endif // DEBUG

	if (touchID == batteryCalibrationTouchPassword[batteryCalibrationTouchPasswordCounter])
	{
		batteryCalibrationTouchPasswordCounter++;
		if (batteryCalibrationTouchPasswordCounter == 5)
		{
			Serial.println("Calibrating battery");
			robot.battery.store = 1;
			batteryCalibrationTouchPasswordCounter = 0;
			robot._setLed(YELLOW, BLACK, 2, 0);
			robot._setLedRotationSpeed(5);
			delay(500);
			robot.setMode(exitMode);
		}
	}
	else
	{
		//false key, exit
		batteryCalibrationTouchPasswordCounter = 0;
		robot.setMode(exitMode);
	}
}

