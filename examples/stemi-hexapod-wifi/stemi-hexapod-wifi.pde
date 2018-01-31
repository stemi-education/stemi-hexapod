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
#include <NeoPixelBus.h>

#include "touchDriver.h"

void setup()
{
	Serial.begin(115200);

}
void loop()
{
	Serial.println("Krenuo!");

	Touch touch(50, 40, 5);

	Robot robot; // initalise the robot
	
	robot.wakeUp(); //wake up the robot

	int robotMode = 0; //0=walk, 1=dance
	int nModes = 4; //number of different modes
	int moveSpeed = 0; //moving speed
	while(1) //repeat following commands forever
	{
		touch.checkTouch();
		if (touch.isTouchDetected())
		{
			int touchState = touch.getTouchPattern(true);
			if (touchState == 2)
				robotMode = (robotMode + 1) % nModes;
			if (robotMode == 0)
			{
				if (touchState == 1)
					robot.body.tr[2] = robot.body.saturate((robot.body.tr[2] - 3), -3, 3);
				if (touchState == 4)
					robot.body.tr[2] = robot.body.saturate((robot.body.tr[2] + 3), -3, 3);
			}
			if (robotMode == 2)
			{
				robot.body.tr[2] = 0;
				robot.ctrl.gaitID = 3;
				robot.body.setGaitUpDown(robot.body.gait.selectSequence(robot.ctrl.gaitID));
				if (touchState == 1)
					moveSpeed = robot.body.saturate(moveSpeed - 5, -5, 5);
				if (touchState == 4)
					moveSpeed = robot.body.saturate(moveSpeed + 5, -5, 5);
			}
		}
		//Serial.print("mode: ");
		//Serial.println(robotMode);
		robot.hardware.wifiRead(); // read package from serial port if available (wifi)
		
		if (robotMode == 0)
		{
			//walk
			robot.body.setCommand(); // set command from the package
			//robot.body.setMoveParam(5, PI / 2, 0.3, 100);
			robot.go(); //run the algorithm		
			robot.hardware.setAllLEDsRainbow(100);
			//Serial.println("banana");
			//robot.hardware.batteryStatus();
			//Serial.println(robot.hardware.batteryStatus());
		}
		else if (robotMode == 1)
		{
			//dance
			robot.hardware.setAllLEDs(100,RgbColor(0, 0, 255));
			float pitch;
			float hipRotation = 0.15, hipTranslation = 0, rithm = 1;
			
			
			for (int i = 0; i < random(4, 8); i++)
			{
				pitch = random(-5, 5);
				pitch /= (float)10;
				//second
				robot.setRotation(pitch, 0, 0);
				robot.danceHip(hipRotation, hipTranslation, 0.5);
				robot.danceHip(-hipRotation, hipTranslation, 0.5);
				robot.danceHip(hipRotation, hipTranslation, 0.5);
				robot.danceHip(-hipRotation, hipTranslation, 0.5);
			}
			robot.resetPose();
		}
		else if (robotMode == 2)
		{
			robot.hardware.setAllLEDs(100, RgbColor(0, 255, 0));
			//walk offline
			robot.body.setMoveParam(moveSpeed, PI / 2, 0, 100);

			//robot.body.setCommand(); // set command from the package
			//robot.body.setMoveParam(5, PI / 2, 0.3, 100);
			Serial.println(robot.ctrl.gaitID);
			robot.go(); //run the algorithm		
			//robot.hardware.setAllLEDsRainbow(100);
		}
		else if (robotMode == 3)
		{
			//move
			robot.hardware.setAllLEDs(100, RgbColor(255, 0, 0));
		}

	}
}
