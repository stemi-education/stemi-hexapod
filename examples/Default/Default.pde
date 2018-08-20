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

#include "Hexapod.h"
SharedData robot;
Hexapod  hexapod;
void setup()
{
	Serial.begin(115200);
	hexapod.init();
	robot.setLed(GREEN);
	robot.setHeight(50);
}

Color clrArray[7] = {BLUE, YELLOW, GREEN, CYAN, PURPLE, RED, ORANGE};
uint8_t clrCount = 0;

void loop()
{

	int touchPattern = robot.getTouchPattern();
	if (touchPattern == TOUCH_00X)
	{
		setLEDrandom();
	}
	else if (touchPattern == TOUCH_X00)
	{
		setLEDSequence();
	}
	else if (touchPattern == TOUCH_0X0)
	{
		robot.exitUserMode();
	}
	else if (touchPattern == TOUCH_0XX)
	{
		robot.setLed(RED, RED, 50, 0);
		robot.setLedRotationSpeed(0);
		robot.setLedBlinkingSpeed(30);
	}
	else if (touchPattern == TOUCH_XX0)
	{
		robot.setLed(RED, WHITE, 50, 0);
		robot.setLedRotationSpeed(30);
		robot.setLedBlinkingSpeed(0);
	}
	delay(20);
}


void setLEDrandom()
{
	robot.setLedStatic(0, RED);
	robot.setLedStatic(1, WHITE);
	robot.setLedStatic(2, BLUE);
	robot.setLedStatic(3, BLUE);
	robot.setLedStatic(4, WHITE);
	robot.setLedStatic(5, RED);
}

void setLEDSequence()
{
	robot.setLedStatic(0, RED);
	robot.setLedStatic(1, WHITE);
	robot.setLedStatic(2, RED);
	robot.setLedStatic(3, WHITE);
	robot.setLedStatic(4, RED);
	robot.setLedStatic(5, WHITE);
}