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


#include "LedDriver.h"


LedDriver::LedDriver(): strip(LED_COUNT, LED_PIN)
{
	strip.Begin();
	strip.SetBrightness(255);
	strip.SetPixelColor(0, RgbColor(0, 0, 0));
	strip.SetPixelColor(1, RgbColor(0, 0, 0));
	strip.SetPixelColor(2, RgbColor(0, 0, 0));
	strip.SetPixelColor(3, RgbColor(0, 0, 0));
	strip.SetPixelColor(4, RgbColor(0, 0, 0));
	strip.SetPixelColor(5, RgbColor(0, 0, 0));
	strip.SetPixelColor(6, RgbColor(0, 0, 0));
	delay(1);
	strip.Show();
}

void LedDriver::setColorParametric()
{
	applyDirectionSpeed();
	applyBlinkingSpeed();

	float gaussWidth = robot.ledCtrl.spreadRatio;
	float directionOffset = PI / 3 - PI;

	int gauss0, gauss1, gauss2;
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			gauss0 = robot.ledCtrl.secondarClr[j] +
				((int)robot.ledCtrl.primarClr[j] - (int)robot.ledCtrl.secondarClr[j])
				* exp(-pow((ledMapInverse[i] * 2 * PI / 6 - (robot.ledCtrl.direction + directionOffset + rotationSpeedDirection)), 2) / (gaussWidth));
			gauss1 = robot.ledCtrl.secondarClr[j] +
				((int)robot.ledCtrl.primarClr[j] - (int)robot.ledCtrl.secondarClr[j])
				* exp(-pow((ledMapInverse[i] * 2 * PI / 6 - (robot.ledCtrl.direction + directionOffset + rotationSpeedDirection) + 2 * PI), 2) / (gaussWidth));
			gauss2 = robot.ledCtrl.secondarClr[j] +
				((int)robot.ledCtrl.primarClr[j] - (int)robot.ledCtrl.secondarClr[j])
				* exp(-pow((ledMapInverse[i] * 2 * PI / 6 - (robot.ledCtrl.direction + directionOffset + rotationSpeedDirection) - 2 * PI), 2) / (gaussWidth));
			robot.ledCtrl.finalClr[i][j] = robot.ledCtrl.primarClr[j] > robot.ledCtrl.secondarClr[j] ?
																			blinkSpeedResult * max(gauss0, max(gauss1, gauss2)) :
																			blinkSpeedResult * min(gauss0, min(gauss1, gauss2));
		}
	}
}
void LedDriver::setColorManual()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			robot.ledCtrl.finalClr[i][j] = robot.ledCtrl.manualClr[i][j];
		}
	}
}

void LedDriver::applyDirectionSpeed() 
{
	if (robot.ledCtrl.rotationSpeed == 0)
		rotationSpeedDirection = 0;
	else
	{

		rotationSpeedDirection += robot.ledCtrl.rotationSpeed * 2 * PI * TASK_PERIOD_LED / 1000;
		//wrap to +-PI
		if (rotationSpeedDirection > PI)
			rotationSpeedDirection = rotationSpeedDirection - 2 * PI* ceil(rotationSpeedDirection / (2 * PI));
		else if (rotationSpeedDirection < -PI)
			rotationSpeedDirection = rotationSpeedDirection - 2 * PI* floor(rotationSpeedDirection / (2 * PI));
	}
}
void LedDriver::applyBlinkingSpeed() 
{
	if (robot.ledCtrl.blinkingSpeed == 0)
	{
		blinkingSpeedPhase = 0;
		blinkSpeedResult = 1;
	}
	else
	{
		blinkingSpeedPhase += robot.ledCtrl.blinkingSpeed * 2 * PI * TASK_PERIOD_LED / 1000;
		if (blinkingSpeedPhase > PI)
			blinkingSpeedPhase = blinkingSpeedPhase - 2 * PI* ceil(blinkingSpeedPhase / (2 * PI));
		else if (blinkingSpeedPhase < -PI)
			blinkingSpeedPhase = blinkingSpeedPhase + 2 * PI* floor(blinkingSpeedPhase / (2 * PI));
		blinkSpeedResult = 0.5 + cos(blinkingSpeedPhase) / 2;
	}
}

//TODO LED color /parameter randomizator

void LedDriver::writeToLED()
{
	for (int i = 0; i < 6; i++)
			strip.SetPixelColor(ledMap[i], RgbColor(robot.ledCtrl.finalClr[i][0], robot.ledCtrl.finalClr[i][1], robot.ledCtrl.finalClr[i][2]));
	strip.SetPixelColor(6, RgbColor(robot.userInputData.ledManualClr[6][0], robot.userInputData.ledManualClr[6][1], robot.userInputData.ledManualClr[6][2]));
			
	delay(3);
	strip.Show();
}
