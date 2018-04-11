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
#include "Arduino.h"

#include "Hexapod.h"
#include "Robot.h"

void walkingEngine(void *sharedData) 
{
	Robot robot((SharedData*)sharedData);
	robot.wakeUp();

	int b = ((SharedData*)sharedData)->a;
	Serial.print("primljeno: ");
	Serial.println(b);
	//robot.hardware.blinkLED(3, 3, 50, 100);

	while (1) //repeat following commands forever
	{
		////check all inputs and change state and variables accordingly////
		//robot.checkTouch();
		//wifi input
		//robot.hardware.wifiRead(); // read wifi package if available
		////execute actions based on state and variables////
		robot.modeGo();
	}
}

Hexapod::Hexapod()
{
	Serial.begin(115200);
	Serial.println("Hexapod init");

	Serial.print("primljeno prije: ");
	Serial.println(sharedData.a);
	xTaskCreatePinnedToCore(walkingEngine, "walkingEngine", 10*4096, (void*)&sharedData, 1, NULL, ARDUINO_RUNNING_CORE);
}