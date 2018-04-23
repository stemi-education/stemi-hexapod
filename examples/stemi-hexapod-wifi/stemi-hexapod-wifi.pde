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

	robot.setHeight(4);
	delay(1000);
}

void loop()
{
	robot.setLed(LED_R2, BLUE);
	robot.rotate(FORWARD);
	delay(2000);

	robot.setLed(RED,GREEN);
	robot.stretch(BACKWARD);
	delay(2000);

	robot.setLed(LED_L2, GREEN);
	robot.rotate(LEFT);
	delay(2000);

	robot.setLed(RED, BLUE, 2, -60);
	robot.stretch(RESET);
	robot.rotate(RIGHT);
	delay(2000);

	robot.setLed(RED, BLUE, 2, 0);
	robot.rotate(RESET);
	robot.setLedRotationSpeed(0.5);
	delay(2000);

	robot.setLedBlinkingSpeed(1);
	delay(5000);





	robot.mode = ROBOT_STANDBY_MODE;
	delay(9999999);
	robot.mode = ROBOT_STANDBY_MODE;
}
