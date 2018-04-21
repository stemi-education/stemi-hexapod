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

void SharedData::setLedColor(Color primarClr, Color secondarClr = BLACK, float spreadRatio = 100, float direction = 0)
{
	ledCtrl.primarClr[0] = primarClr.r;
	ledCtrl.primarClr[1] = primarClr.g;
	ledCtrl.primarClr[2] = primarClr.b;

	ledCtrl.secondarClr[0] = secondarClr.r;
	ledCtrl.secondarClr[1] = secondarClr.g;
	ledCtrl.secondarClr[2] = secondarClr.b;

	ledCtrl.spreadRatio = spreadRatio;

	ledCtrl.direction = direction * PI / 180 + PI/2;
}

void SharedData::setLedRotationSpeed(float rotationSpeed)
{
}

void SharedData::setLedBlinkingSpeed(float blinkingSpeed)
{
}


