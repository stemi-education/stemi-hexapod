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


#include "SharedData.h"
#include "Server.h"
#include <WiFi.h>
#include <esp_wifi.h>

SharedData:: SharedData()
{
	Serial.begin(9600);

	moveCtrl.linearVelocity = 0;
	moveCtrl.direction = PI / 2;
	moveCtrl.angularVelocity = 0;
	moveCtrl.poseVector[0] = 1;
	moveCtrl.poseVector[1] = 0; 
	moveCtrl.poseVector[2] = 0; 
	moveCtrl.poseVector[3] = 0; 
	moveCtrl.poseVector[4] = 0; 
	moveCtrl.poseVector[5] = 0; 
	moveCtrl.timeout = -1;
	
	//init for move()

	userInputData.translationZ = 0;
	userInputData.ledMode = LED_PARAMETRIC_MODE;
	userInputData.ledSpreadRatio = 100;
	userInputData.poseSpeed = 60;
	userInputData.moveDuration = -1;

	btInputData.translationZ = 50;
	btInputData.ledMode = LED_PARAMETRIC_MODE;
	btInputData.ledPrimarClr[2] = 255; //set initial color to blue
	btInputData.ledSpreadRatio = 100;
	btInputData.poseSpeed = 60;
	btInputData.moveDuration = -1;

	danceInputData.translationZ = 50;
	danceInputData.ledMode = LED_PARAMETRIC_MODE;
	danceInputData.ledSpreadRatio = 100;
	danceInputData.poseSpeed = 80;
	danceInputData.moveDuration = -1;
}

void SharedData::writeServoAngles(float servoAnglesNew[18])
{
	for (int i = 0; i < 18; i++)
		servoCtrl.servoAngles[i] = servoAnglesNew[i];
}

//use input from pointed structure [bluetooth,user]
void SharedData::useGeneralInputData(InputData *data)
{//TODO sperate this to multiple functions
	//moveCtrl.timeout = data->moveTimeout;
	servoCtrl.power = data->servoPower;
}

void SharedData::useMoveInputData(InputData *data)
{
	//move
	moveCtrl.linearVelocity = data->linearVelocity / 100.0 * 10; //[0,100]% -> [0, 10]cm/s
	moveCtrl.direction = data->direction * PI / 180.0 + PI / 2; //[-180,180]degree -> [-PI,PI]rad
	moveCtrl.angularVelocity = data->angularVelocity / 100.0 * 0.7; //[-100,100]% -> [-0.7, 0.7]rad/s

	moveCtrl.poseVector[0] = data->translationZ / 100.0 * 7 + 1;
	moveCtrl.poseVector[1] = data->translationX / 100.0 * 3;
	moveCtrl.poseVector[2] = data->translationY / 100.0 * 3;
	moveCtrl.poseVector[3] = data->rotationZ / 100.0 * 0.3;
	moveCtrl.poseVector[4] = data->rotationX / 100.0 * 0.3;
	moveCtrl.poseVector[5] = data->rotationY / 100.0 * 0.3;

	_writeExtraServo(data->extraServoAngle);

	PMParam.gaitID = data->gaitID;
	PMParam.stepHeight = data->stepHeight / 100.0 * 4;
	PMParam.poseChangeSpeed = 0.99 - (0.00014 / -0.055)*(1 - exp(0.055*data->poseSpeed));

	if (data->moveDuration > 0)
	{
		moveCtrl.timeout = data->moveDuration * PMParam.freq / 1000;
		data->moveDuration = 0;
	}
	else if (data->moveDuration == -1)
		moveCtrl.timeout = -1;
}

void SharedData::useLedInputData(InputData *data)
{
	//led
	Color primarClr = { data->ledPrimarClr[0] , data->ledPrimarClr[1] , data->ledPrimarClr[2] };
	Color secondarClr = { data->ledSecondarClr[0] , data->ledSecondarClr[1] , data->ledSecondarClr[2] };
	_setLed(primarClr, secondarClr,
		0.000121716*pow(data->ledSpreadRatio, 2.584963),
		data->ledDiretion * PI / 180 + PI / 2);
	_setLedRotationSpeed(data->ledRotationSpeed / 100.0 * 10);
	_setLedBlinkingSpeed(data->ledBlinkingSpeed / 100.0 * 10);
	if (!robot.isSerialConnectionOn) {
		ledCtrl.mode = data->ledMode;
		for (int i = 0; i < LED_COUNT; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ledCtrl.manualClr[i][j] = data->ledManualClr[i][j];
			}
		}
	}
}

//seting user data [public]
void SharedData::setLedStatic(uint8_t ledNo, Color color)
{
	userInputData.ledMode = LED_MANUAL_MODE;
	userInputData.ledManualClr[ledNo][0] = color.r;
	userInputData.ledManualClr[ledNo][1] = color.g;
	userInputData.ledManualClr[ledNo][2] = color.b;
}

void SharedData::setLedStatic(Color color)
{
	userInputData.ledMode = LED_MANUAL_MODE;
	for (int i = 0; i < LED_COUNT; i++)
	{
		userInputData.ledManualClr[i][0] = color.r;
		userInputData.ledManualClr[i][1] = color.g;
		userInputData.ledManualClr[i][2] = color.b;
	}
}

void SharedData::_setLedStatic(uint8_t ledNo, Color color)
{
	ledCtrl.mode = LED_MANUAL_MODE;
	ledCtrl.manualClr[ledNo][0] = color.r;
	ledCtrl.manualClr[ledNo][1] = color.g;
	ledCtrl.manualClr[ledNo][2] = color.b;
}

void SharedData::_setLedStatic(Color color)
{
	ledCtrl.mode = LED_MANUAL_MODE;
	for (int i = 0; i < LED_COUNT; i++)
	{
		ledCtrl.manualClr[i][0] = color.r;
		ledCtrl.manualClr[i][1] = color.g;
		ledCtrl.manualClr[i][2] = color.b;
	}
}

void SharedData::setLed(Color color)
{
	userInputData.ledMode = LED_PARAMETRIC_MODE;
	setLed(color, color, 50, 0);	
}

void SharedData::_setLed(Color color)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	_setLed(color, color, 50, 0);
}

void SharedData::setLed(Color primarClr, Color secondarClr, uint8_t spreadRatio, int16_t direction)
{
	userInputData.ledMode = LED_PARAMETRIC_MODE;
	userInputData.ledPrimarClr[0] = primarClr.r;
	userInputData.ledPrimarClr[1] = primarClr.g;
	userInputData.ledPrimarClr[2] = primarClr.b;

	userInputData.ledSecondarClr[0] = secondarClr.r;
	userInputData.ledSecondarClr[1] = secondarClr.g;
	userInputData.ledSecondarClr[2] = secondarClr.b;
	userInputData.ledSpreadRatio = spreadRatio;
	userInputData.ledDiretion = direction;
}

void SharedData::_setLed(Color primarClr, Color secondarClr, float spreadRatio, float direction)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	ledCtrl.primarClr[0] = primarClr.r;
	ledCtrl.primarClr[1] = primarClr.g;
	ledCtrl.primarClr[2] = primarClr.b;

	ledCtrl.secondarClr[0] = secondarClr.r;
	ledCtrl.secondarClr[1] = secondarClr.g;
	ledCtrl.secondarClr[2] = secondarClr.b;
	ledCtrl.spreadRatio = spreadRatio;
	ledCtrl.direction = direction;
}

void SharedData::setLedRotationSpeed(int8_t rotationSpeed)
{
	userInputData.ledRotationSpeed = rotationSpeed;
}

void SharedData::_setLedRotationSpeed(float rotationSpeed)
{
	ledCtrl.rotationSpeed = rotationSpeed;
}

void SharedData::setLedBlinkingSpeed(uint8_t blinkingSpeed)
{
	userInputData.ledBlinkingSpeed = blinkingSpeed;
}

void SharedData::_setLedBlinkingSpeed(float blinkingSpeed)
{
	ledCtrl.blinkingSpeed = blinkingSpeed;
}

void SharedData::move(userPresetInputData movement, float duration)
{
	userInputData.linearVelocity = saturate(movement.linearVelocity,0,100);
	userInputData.direction = movement.direction;
	userInputData.moveDuration = duration;
}

void SharedData::_move(float linearVelocity, float direction, float angularVelocity, float duration)
{
	moveCtrl.linearVelocity = saturate(linearVelocity, 0, 15);
	moveCtrl.direction = direction;
	moveCtrl.angularVelocity = saturate(angularVelocity, -1, 1);
	moveCtrl.timeout = duration * PMParam.freq / 1000;
}

void SharedData::move(uint8_t linearVelocity, int16_t direction, int8_t angularVelocity, float duration)
{
	userInputData.linearVelocity = saturate(linearVelocity, 0, 100);
	userInputData.direction = direction;
	userInputData.angularVelocity = saturate(angularVelocity, -100, 100);
	userInputData.moveDuration = duration;
}

void SharedData::rotate(userPresetInputData rotation, float duration)
{
	userInputData.angularVelocity = saturate(rotation.angularVelocity, -100, 100);
	userInputData.moveDuration = duration;
}

void SharedData::tilt(userPresetInputData tiltation)
{
	userInputData.rotationX = saturate(tiltation.rotationX, -100, 100);
	userInputData.rotationY = saturate(tiltation.rotationY, -100, 100);
	userInputData.rotationZ = saturate(tiltation.rotationZ, -100, 100);
}

void SharedData::tilt(int8_t rotationXNew, int8_t rotationYNew, int8_t rotationZNew)
{
	userInputData.rotationX = saturate(rotationXNew, -100, 100);
	userInputData.rotationY = saturate(rotationYNew, -100, 100);
	userInputData.rotationZ = saturate(rotationZNew, -100, 100);
}

void SharedData::stretch(userPresetInputData stretchment)
{
	userInputData.translationX = saturate(stretchment.translationX, -100, 100);
	userInputData.translationY = saturate(stretchment.translationY, -100, 100);
}

void SharedData::stretch(int8_t translationXNew, int8_t treanslationYNew)
{
	userInputData.translationX = saturate(translationXNew, -100, 100);
	userInputData.translationY = saturate(treanslationYNew, -100, 100);
}

void SharedData::setHeight(uint8_t height)
{
	userInputData.translationZ = saturate(height, 0, 100);
}

void SharedData::_setHeight(float height)
{
	moveCtrl.poseVector[0] = saturate(height, 0, 8);
}

void SharedData::_setPose(float poseVectorNew[6])
{
	moveCtrl.poseVector[0] = saturate(poseVectorNew[0], 0, 8);
	moveCtrl.poseVector[1] = saturate(poseVectorNew[1], -STRETCH_MAX, STRETCH_MAX);
	moveCtrl.poseVector[2] = saturate(poseVectorNew[2], -STRETCH_MAX, STRETCH_MAX);
	moveCtrl.poseVector[3] = saturate(poseVectorNew[3], -TILT_MAX, TILT_MAX);
	moveCtrl.poseVector[4] = saturate(poseVectorNew[4], -TILT_MAX, TILT_MAX);
	moveCtrl.poseVector[5] = saturate(poseVectorNew[5], -TILT_MAX, TILT_MAX);
}

void SharedData::setMode(int8_t modeNew)
{
	mode = modeNew;
	moveCtrl.timeout = 0;
}

int8_t SharedData::getMode()
{
	return mode;
}

void SharedData::storeServoCalibrationData()
{
	robot.servoCtrl.store = 1;
}

void SharedData::setServoPower(bool power)
{
	userInputData.servoPower = power;
}

void SharedData::_setServoPower(bool power)
{
	servoCtrl.power = power;
}

void SharedData::writeExtraServo(float servoAngle)
{
	userInputData.extraServoAngle = servoAngle;
}

void SharedData::_writeExtraServo(float servoAngle)
{
	servoCtrl.extraServoAngle = servoAngle;
}

uint8_t SharedData::getTouchPattern()
{
	if (mode != ROBOT_USER_MODE)
		return 0; //not allowd to use while not in ROBOT_USER_MODE
	uint8_t touchState = touch.state;
	touch.state = TOUCH_000; //reset the touch once it has been used
#ifdef DEBUG_
	if (touchState != 0)
	{
		Serial.print("t: ");
		Serial.println(touchState);
	}
#endif // DEBUG_

	
	return touchState;
}

uint8_t SharedData::_getTouchPattern()
{
	if (mode == ROBOT_USER_MODE)
		return 0; //not allowd to use while in ROBOT_USER_MODE
	uint8_t touchState = touch.state;
	touch.state = TOUCH_000; //reset the touch once it has been used
#ifdef DEBUG_
	if (touchState != 0)
	{
		Serial.print("t: ");
		Serial.println(touchState);
	}
#endif // DEBUG_
	return touchState;
}

void SharedData::_setTouchPattern(uint8_t touchState)
{
	touch.state = touchState;
}

float SharedData::getBatteryVoltage()
{
	return robot.battery.voltage;
}

uint8_t SharedData::getBatteryPercentage()
{
	return robot.battery.percentage;
}

void SharedData::enterUserMode()
{
	robot.setMode(ROBOT_USER_MODE);
	moveCtrl.timeout = 0;
}

void SharedData::exitUserMode()
{
	robot.setMode(ROBOT_WALK_MODE);
	moveCtrl.timeout = 0;
}

void SharedData::loadName()
{
	// Preferences error on some ESP32 devices
	uint8_t mac[6];
	esp_efuse_mac_get_default(mac);
	name = names.generateName(names.sumStringMemberValues1(mac), names.sumStringMemberValues2(mac));
	Serial.printf("Name not stored, storing \"%s\"\n",name.c_str());
	return;
	names.storeInit();
	names.load(&robot.name);
	if (name == "\0")
	{
		esp_efuse_mac_get_default(mac);
		name = names.generateName(names.sumStringMemberValues1(mac), names.sumStringMemberValues2(mac));
		Serial.printf("Name not stored, storing \"%s\"\n",name.c_str());
		storeName(name);
	}
}

void SharedData::storeName(std::string nameNew)
{
	names.storeInit();
	names.store(nameNew);
}

void SharedData::startAction(std::string actionName)
{
	if (actionName == "ota") {
		startOtaServer();
	}
}