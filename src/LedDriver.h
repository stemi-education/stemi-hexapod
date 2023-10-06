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


#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include <NeoPixelBrightnessBus.h>
#include "SharedData.h"

class LedDriver
{
public:
	LedDriver();
	
	void setColorParametric();
	void setColorManual();
	void applyDirectionSpeed();
	void applyBlinkingSpeed();
	void writeToLED();

	NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip;

	//LED internal parameters
	float rotationSpeedDirection = 0;
	float blinkingSpeedPhase = 0;
	float blinkSpeedResult = 1;

	//Colors
	RgbColor red = RgbColor(255, 0, 0);
	RgbColor green = RgbColor(0, 255, 0);
	RgbColor blue = RgbColor(0, 0, 255);
	RgbColor yellow = RgbColor(255, 242, 0);
	RgbColor purple = RgbColor(255, 0, 255);
	RgbColor white = RgbColor(255);
	RgbColor black = RgbColor(0);

	uint8_t ledMap[7] = { 3, 4, 5, 0, 1, 2, 6};
	uint8_t ledMapInverse[7] = { 5, 4, 3, 2, 1, 0, 6};


};
#endif
