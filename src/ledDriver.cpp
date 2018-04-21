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


LedDriver::LedDriver(SharedData *sharedDataNew): strip(LED_COUNT, LED_PIN)
{
	sharedData = sharedDataNew;
	strip.Begin();
	strip.SetBrightness(255);
	strip.SetPixelColor(0, RgbColor(255, 0, 0));
	strip.SetPixelColor(1, RgbColor(100, 100, 0));
	strip.SetPixelColor(2, RgbColor(0, 255, 0));
	strip.SetPixelColor(3, RgbColor(0, 100, 100));
	strip.SetPixelColor(4, RgbColor(0, 0, 255));
	strip.SetPixelColor(5, RgbColor(100, 0, 100));
	delay(1);
	strip.Show();
}

void LedDriver::setColorParametric()
{
	applyDirectionSpeed();
	applyBlinkingSpeed();

	float gaussWidth = sharedData->ledCtrl.spreadRatio;
	float directionOffset = -PI / 3 + PI;

	int gauss0, gauss1, gauss2;
	for (int i = 0; i < LED_COUNT; i++) {
		for (int j = 0; j < 3; j++) {
			gauss0 = sharedData->ledCtrl.secondarClr[j] +
				(sharedData->ledCtrl.primarClr[j] - sharedData->ledCtrl.secondarClr[j])
				* exp(-pow((-i * 2 * PI / LED_COUNT - (sharedData->ledCtrl.direction + directionOffset + rotationSpeedDirection)), 2) / (2 * gaussWidth));
			gauss1 = sharedData->ledCtrl.secondarClr[j] +
				(sharedData->ledCtrl.primarClr[j] - sharedData->ledCtrl.secondarClr[j])
				* exp(-pow((-i * 2 * PI / LED_COUNT - (sharedData->ledCtrl.direction + directionOffset + rotationSpeedDirection) + 2 * PI), 2) / (2 * gaussWidth));
			gauss2 = sharedData->ledCtrl.secondarClr[j] +
				(sharedData->ledCtrl.primarClr[j] - sharedData->ledCtrl.secondarClr[j])
				* exp(-pow((-i * 2 * PI / LED_COUNT - (sharedData->ledCtrl.direction + directionOffset + rotationSpeedDirection) - 2 * PI), 2) / (2 * gaussWidth));
			sharedData->ledCtrl.manualClr[i][j] = blinkSpeedResult * max(gauss0, max(gauss1, gauss2));
		}
	}
	writeToLED();
}

float LedDriver::applyDirectionSpeed() {
	if (sharedData->ledCtrl.rotationSpeed == 0)
		rotationSpeedDirection = 0;
	else
	{

		rotationSpeedDirection += sharedData->ledCtrl.rotationSpeed;
		//wrap to +-PI
		if (rotationSpeedDirection > PI)
			rotationSpeedDirection = rotationSpeedDirection - 2 * PI* ceil(rotationSpeedDirection / (2 * PI));
		else if (rotationSpeedDirection < -PI)
			rotationSpeedDirection = rotationSpeedDirection + 2 * PI* floor(rotationSpeedDirection / (2 * PI));
	}
}
float LedDriver::applyBlinkingSpeed() {
	if (sharedData->ledCtrl.blinkingSpeed == 0)
	{
		blinkingSpeedPhase = 0;
		blinkSpeedResult = 1;
	}
	else
	{
		blinkingSpeedPhase += sharedData->ledCtrl.blinkingSpeed;
		if (blinkingSpeedPhase > PI)
			blinkingSpeedPhase = blinkingSpeedPhase - 2 * PI* ceil(blinkingSpeedPhase / (2 * PI));
		else if (blinkingSpeedPhase < -PI)
			blinkingSpeedPhase = blinkingSpeedPhase + 2 * PI* floor(blinkingSpeedPhase / (2 * PI));
		blinkSpeedResult = 0.5 + cos(blinkingSpeedPhase) / 2;
		Serial.print("bsp: ");
		Serial.print(blinkingSpeedPhase);
		Serial.print("r: ");
		Serial.print(blinkSpeedResult);
	}
}

void LedDriver::writeToLED()
{
	for (int i = 0; i < LED_COUNT; i++)
			strip.SetPixelColor(i, RgbColor(sharedData->ledCtrl.manualClr[i][0], sharedData->ledCtrl.manualClr[i][1], sharedData->ledCtrl.manualClr[i][2]));
	delay(1);
	strip.Show();
}
