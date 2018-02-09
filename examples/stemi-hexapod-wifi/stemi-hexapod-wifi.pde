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

	Serial.println("Krenuo!");

	Touch touch(50, 40, 5); // initialize touch sensors

	Robot robot; // initalise the robot

	robot.wakeUp(); //wake up the robot

	int robotMode = WALKING_MODE; //start with basic wlking mode that listens to wifi
	int moveSpeed = 0; //moving speed
	int hipMode = 0; //blue mode moving parameter

	int calibrationLegSelected = 0, calibrationServoLayerSelected = 0, calibrationValue = 125;
	bool nudgeServos = 0;
	while (1) //repeat following commands forever
	{
		////check all inputs and change state and variables accordingly////

		//touch input
		touch.checkTouch();
		if (touch.isTouchDetected())
		{
			int touchState = touch.getTouchPattern(true);			

			//make changes based on input and current robotMode
			switch (robotMode)
			{
			case PRE_CALIBRATION_MODE:
				switch (touchState)
				{
				case 5:
					robotMode = CALIBRATION_MODE;
					calibrationLegSelected = 0;
					calibrationServoLayerSelected = 0;
					break;
				default:
					robotMode = WALKING_MODE;
					break;
				}
				break;
			case CALIBRATION_MODE:
				switch (touchState)
				{
					//changing calibration value
				case 1:
					calibrationValue = robot.body.saturate((calibrationValue - 25), 0, 250);
					break;
				case 4:
					calibrationValue = robot.body.saturate((calibrationValue + 25), 0, 250);
					break;
					//changing selected servo
				case 3:
					calibrationServoLayerSelected = (calibrationServoLayerSelected + 1) % 3;
					nudgeServos = 1;
					break;
				case 2:
					calibrationLegSelected = (calibrationLegSelected + 1) % 6;
					break;
					//state change
				case 5:
					robotMode = WALKING_MODE;
					robot.body.setLinMode(0);
					break;
				}
				break;
			case WALKING_MODE:
				switch (touchState)
				{
				case 1:
					robot.body.tr[2] = robot.body.saturate((robot.body.tr[2] - 3), -3, 3);
					break;
				case 4:
					robot.body.tr[2] = robot.body.saturate((robot.body.tr[2] + 3), -3, 3);
					break;
					//state change
				case 2:
					robotMode = (robotMode + 1) % NUMBER_OF_MODES;
					break;
				case 5:
					robotMode = PRE_CALIBRATION_MODE;
					break;
				}
				break;
			case OFFLINE_MODE:
				robot.body.tr[2] = 0;
				robot.ctrl.gaitID = 3;
				robot.body.setGaitUpDown(robot.body.gait.selectSequence(robot.ctrl.gaitID));
				switch (touchState)
				{
				case 1:
					hipMode = -1;
					break;
				case 4:
					hipMode = 1;
					break;
					//state change
				case 2:
					robotMode = (robotMode + 1) % NUMBER_OF_MODES;
					break;
				case 5:
					robotMode = PRE_CALIBRATION_MODE;
					break;
				}
				break;
			case DANCING_MODE:
				switch (touchState)
				{
				case 1:
					hipMode = -1;
					break;
				case 4:
					hipMode = 1;
					break;
				//state change
				case 2:
					robotMode = (robotMode + 1) % NUMBER_OF_MODES;
					break;
				case 5:
					robotMode = PRE_CALIBRATION_MODE;
					break;
				}
				break;
			case RANDOM_MODE:
				switch (touchState)
				{
				//state change
				case 2:
					robotMode = (robotMode + 1) % NUMBER_OF_MODES;
					break;
				case 5:
					robotMode = PRE_CALIBRATION_MODE;
					break;
				}
				break;
			}
			Serial.print("mode: ");
			Serial.println(robotMode);
		}


		//wifi input
		robot.hardware.wifiRead(); // read package from serial port if available (wifi)

		////execute actions based on state and variables////

		switch (robotMode)
		{
		case PRE_CALIBRATION_MODE:
			robot.hardware.setAllLEDs(50, RgbColor(255, 0, 0));
			break;
		case CALIBRATION_MODE:
		{
			robot.hardware.strip.SetBrightness(255);
			for (int i = 0; i < 6; i++)
			{
				if (i == calibrationLegSelected)
				{
					int LEDbrightness = calibrationServoLayerSelected * 115 + 25;
					robot.hardware.strip.SetPixelColor(i, RgbColor(LEDbrightness, LEDbrightness, LEDbrightness));
				}
				else
					robot.hardware.strip.SetPixelColor(i, RgbColor(calibrationValue, 0, 0));
			}
			delay(1);
			robot.hardware.strip.Show();

			robot.body.setLinMode(1);
			if (nudgeServos)
			{
				for (int i = 0; i < 18; i++)
				{
					if (i % 3 == calibrationServoLayerSelected) robot.body.qAll[i] += 0.2;
					Serial.printf("%f ",robot.body.qAll[i]);
				}
				Serial.println();
				robot.hardware.servoWrite(robot.body.qAll);
				delay(300);
				for (int i = 0; i < 18; i++)
				{
					if (i % 3 == calibrationServoLayerSelected) robot.body.qAll[i] -= 0.2;
				}
				robot.hardware.servoWrite(robot.body.qAll);
				delay(300);
				nudgeServos = 0;
			}
			else
				robot.go();

			break;
		}
		case WALKING_MODE:
			//walk
			robot.body.setCommand(); // set command from the package
			robot.go(); //run the algorithm		
			robot.hardware.setAllLEDsRainbow(100);
			//robot.hardware.batteryStatus();
			break;
		case OFFLINE_MODE:
			//offline move
			robot.hardware.setAllLEDs(100, RgbColor(0, 255, 0));

			if (hipMode == -1)
			{
				robot.goBackwards(5);
				robot.goHome();
				hipMode = 0;
			}
			else if (hipMode == 1)
			{
				robot.goForward(5);
				robot.goHome();
				hipMode = 0;
			}
			else
			{
				robot.resetPose();
				robot.goHome();
			}
			break;
		case DANCING_MODE:
			//dance
		{
			float pitch;
			float hipRotation = 0.15, hipTranslation = 0;

			robot.hardware.setAllLEDs(100, RgbColor(0, 0, 255));
			if (hipMode == -1)
			{
				int randomNumber = random(4, 8);
				for (int i = 0; i < randomNumber; i++)
				{
					pitch = random(-5, 5);
					pitch /= (float)10;
					Serial.println(pitch);
					Serial.print(" ran: ");
					Serial.println(randomNumber);
					//second
					robot.setRotation(pitch, 0, 0);
					robot.danceHip(hipRotation, hipTranslation, 0.5);
					robot.danceHip(-hipRotation, hipTranslation, 0.5);
					robot.danceHip(hipRotation, hipTranslation, 0.5);
					robot.danceHip(-hipRotation, hipTranslation, 0.5);
				}
				robot.resetPose();
				hipMode = 0;
			}
			else if (hipMode == 1)
			{
				robot.goForward(5);
				hipMode = 0;
			}
			else
			{
				robot.goHome();
			}
			break;
		}
		case RANDOM_MODE:
			robot.hardware.setAllLEDs(100, RgbColor(255, 255, 0));
			robot.resetPose();
			robot.goHome();
		}
	}
}
void loop()
{

}
