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

SharedData:: SharedData()
{
	moveCtrl.linearVelocity = 0;
	moveCtrl.direction = PI / 2;
	moveCtrl.angularVelocity = 0;
	moveCtrl.poseVector[6] = 1;
	moveCtrl.poseVector[6] = 0; 
	moveCtrl.poseVector[6] = 0; 
	moveCtrl.poseVector[6] = 0; 
	moveCtrl.poseVector[6] = 0; 
	moveCtrl.poseVector[6] = 0; 
	moveCtrl.timeout = -1;
	moveCtrl.gaitID = 1;
	moveCtrl.stepHight = 2;
}

void SharedData::writeServoAngles(float servoAnglesNew[18])
{
	for (int i = 0; i < 18; i++)
		servoCtrl.servoAngles[i] = servoAnglesNew[i];
}

void SharedData::writeBtCtrlToMoveCtrl()
{
	moveCtrl.linearVelocity = btCtrl.linearVelocity;
	moveCtrl.direction = btCtrl.direction;
	moveCtrl.angularVelocity = btCtrl.angularVelocity;
}

void SharedData::setLed(uint8_t ledNo, Color color)
{
	ledCtrl.mode = LED_MANUAL_MODE;
	ledCtrl.manualClr[ledNo][0] = color.r;
	ledCtrl.manualClr[ledNo][1] = color.g;
	ledCtrl.manualClr[ledNo][2] = color.b;
}

void SharedData::setLed(Color primarClr)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	ledCtrl.primarClr[0] = primarClr.r;
	ledCtrl.primarClr[1] = primarClr.g;
	ledCtrl.primarClr[2] = primarClr.b;

	ledCtrl.secondarClr[0] = primarClr.r;
	ledCtrl.secondarClr[1] = primarClr.g;
	ledCtrl.secondarClr[2] = primarClr.b;
	ledCtrl.spreadRatio = 1;
	ledCtrl.direction = PI/2;
}
void SharedData::setLed(Color primarClr, Color secondarClr, float spreadRatio, float direction)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	ledCtrl.primarClr[0] = primarClr.r;
	ledCtrl.primarClr[1] = primarClr.g;
	ledCtrl.primarClr[2] = primarClr.b;

	ledCtrl.secondarClr[0] = secondarClr.r;
	ledCtrl.secondarClr[1] = secondarClr.g;
	ledCtrl.secondarClr[2] = secondarClr.b;
	ledCtrl.spreadRatio = spreadRatio;
	ledCtrl.direction = direction * PI / 180 + PI / 2;
}

void SharedData::setLedRotationSpeed(float rotationSpeed)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	ledCtrl.rotationSpeed = rotationSpeed/10;
}

void SharedData::setLedBlinkingSpeed(float blinkingSpeed)
{
	ledCtrl.mode = LED_PARAMETRIC_MODE;
	ledCtrl.blinkingSpeed = blinkingSpeed/10;
}

void SharedData::move(MoveCtrl movement)
{
	moveCtrl.linearVelocity = movement.linearVelocity;
	moveCtrl.direction = movement.direction;
}

void SharedData::move(float linearVelocity, float direction, float angularVelocity)
{
	moveCtrl.linearVelocity = linearVelocity;
	moveCtrl.direction = direction * PI / 180;
	moveCtrl.angularVelocity = angularVelocity * PI / 180;
}

void SharedData::rotate(MoveCtrl rotation)
{
	moveCtrl.angularVelocity = rotation.angularVelocity;
}

void SharedData::tilt(MoveCtrl tiltation)
{
	moveCtrl.poseVector[4] = tiltation.poseVector[4];
	moveCtrl.poseVector[5] = tiltation.poseVector[5];

}

void SharedData::stretch(MoveCtrl stretchment)
{
	moveCtrl.poseVector[1] = stretchment.poseVector[1];
	moveCtrl.poseVector[2] = stretchment.poseVector[2];
}

void SharedData::setHeight(float height)
{
	moveCtrl.poseVector[0] = height;
}

void SharedData::setMode(int8_t modeNew)
{
	mode = modeNew;
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
	servoCtrl.power = power;
}



