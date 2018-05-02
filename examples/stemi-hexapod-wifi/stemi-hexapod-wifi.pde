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
Hexapod  *hexapod;
void setup()
{
	Serial.begin(115200);
	hexapod = new Hexapod();
	
	//robot.enterUserMode();
	robot.setHeight(50);
	robot.setLed(GREEN);
	}

Color clrArray[6] = {BLUE, YELLOW, GREEN, CYAN, PURPLE, RED};

void loop()
{
	
	int touchPattern = robot.getTouchPattern();
	if (touchPattern == TOUCH_00X)
	{
		robot.move(FORWARD);
		delay(1000);
		robot.move(RESET);
	}
	else if (touchPattern == TOUCH_X00)
	{
		Color randomColor = { random(0,255), random(0,255),  random(0,255) };
		robot.setLed(randomColor);
	}
	else if (touchPattern == TOUCH_0X0)
	{
		robot.exitUserMode();
	}
	delay(20);
}


void setLEDrandom()
{
	robot.setLed(0, clrArray[random(0, 5)]);
	robot.setLed(1, clrArray[random(0, 5)]);
	robot.setLed(2, clrArray[random(0, 5)]);
	robot.setLed(3, clrArray[random(0, 5)]);
	robot.setLed(4, clrArray[random(0, 5)]);
	robot.setLed(5, clrArray[random(0, 5)]);
}

void setLEDrandomDelay(int delayTime)
{
	robot.setLed(0, clrArray[random(0, 5)]);
	delay(delayTime);
	robot.setLed(1, clrArray[random(0, 5)]);
	delay(delayTime);
	robot.setLed(2, clrArray[random(0, 5)]);
	delay(delayTime);
	robot.setLed(3, clrArray[random(0, 5)]);
	delay(delayTime);
	robot.setLed(4, clrArray[random(0, 5)]);
	delay(delayTime);
	robot.setLed(5, clrArray[random(0, 5)]);
	delay(delayTime);
}