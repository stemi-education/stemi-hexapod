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


#include "Robot.h"

Robot::Robot() : body(ctrl, parameters), hardware(ctrl)
{
	go();
}

void Robot::wakeUp()
{
	hardware.wifiInit();
	body.setLinMode(LIN_MODE_PERMANENT);
	go(); //make a first run(), put the legs in the air

	hardware.servoPower(1);

	unsigned long startTime = millis(); //start measuring time
	Serial.println("Waiting for 3 sec ...");
	while (millis() - startTime < 3000) //read serial for 3 seconds
	{
		hardware.wifiRead();
	}

	body.setLinMode(LIN_MODE_PERMANENT);

	//hardware.playSound(10000, 500); //beep a sound f=5KHz, t=500ms

	startTime = millis();
	while (millis() - startTime < 2000) //wait for 2 seconds until the robot becomes responsive (safety reasons)
	{

		hardware.wifiRead(); //read commands to keep the buffer empty and receive LIN packages
		body.resetCommands(); //but ignore movement commands
		go(); //run the algorithm, just to stand up
	}
	Serial.println("STEMI has waken up!");
}

int Robot::go()
{
	body.run();
	wait(); //wait until enough time has passed
	hardware.servoWrite(body.qAll);
	measureTime(); //start measuring time

	return 0;
}

void Robot::measureTime()
{
	startTime = millis();
}

void Robot::wait()
{
	while (millis() - startTime < body.ts * 1000)
	{

	}
	//Serial.println(millis() - startTime);
	if (millis() - startTime > body.ts * 1000)
	{
		Serial.print("This cycle took more than expected: ");
		Serial.println(millis() - startTime);
	}
}

void Robot::loopMove() {
	//Serial.println("pocetak");
	while (ctrl.nMove > 0)
	{
		//Serial.println("before");
		go();
		//Serial.println("after");
	}
}
void Robot::loopHome() {
	while (!body.checkHomeMark())
	{
		ctrl.nMove = 0;
		go();
	}
}
void Robot::goHome()
{
	loopHome();
}
void Robot::goForward(float distance)
{
	distance *= 1.08;
	body.setMoveParam(goSpeed, PI / 2, 0, abs(distance / goSpeed*parameters.freq));
	loopMove();
}

void Robot::goBackwards(float distance)
{
	distance *= 1.08;
	body.setMoveParam(goSpeed, -PI / 2, 0, abs(distance / goSpeed*parameters.freq));
	loopMove();
}

void Robot::goLeft(float distance)
{
	distance *= 1.08;
	body.setMoveParam(goSpeed, PI, 0, abs(distance / goSpeed*parameters.freq));
	loopMove();
}

void Robot::goRight(float distance)
{
	distance *= 1.08;
	body.setMoveParam(goSpeed, 0, 0, abs(distance / goSpeed*parameters.freq));
	loopMove();
}

void Robot::turnLeft(float distance)
{
	distance *= 1.15;
	body.setMoveParam(0, 0, -turnSpeed, abs((distance*PI / 180) / turnSpeed*parameters.freq));
	loopMove();
}

void Robot::turnRight(float distance)
{
	distance *= 1.15;
	body.setMoveParam(0, 0, turnSpeed, abs((distance*PI / 180) / turnSpeed*parameters.freq));
	loopMove();
}
