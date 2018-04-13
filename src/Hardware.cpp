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


#include "Hardware.h"


Hardware::Hardware(SharedData *sharedDataNew) : strip(LED_COUNT, LED_PIN), touch(50, 40, 5)
{
	sharedData = sharedDataNew;

	LEDinit();
}

boolean ByteArrayCompare(char a[], const char b[], int array_size)
{
	for (int i = 0; i < array_size; ++i)
		if (a[i] != b[i])
			return(false);
	return(true);
}

void Hardware::LEDinit()
{
	strip.Begin();
	strip.SetBrightness(50);
	strip.SetPixelColor(0, RgbColor(255, 0, 0));
	strip.SetPixelColor(1, RgbColor(100, 100, 0));
	strip.SetPixelColor(2, RgbColor(0, 255, 0));
	strip.SetPixelColor(3, RgbColor(0, 100, 100));
	strip.SetPixelColor(4, RgbColor(0, 0, 255));
	strip.SetPixelColor(5, RgbColor(100, 0, 100));
	delay(1);
	strip.Show();
}

void Hardware::setAllLEDs(int bright, RgbColor color)
{
	strip.SetBrightness(bright);
	strip.SetPixelColor(0, color);
	strip.SetPixelColor(1, color);
	strip.SetPixelColor(2, color);
	strip.SetPixelColor(3, color);
	strip.SetPixelColor(4, color);
	strip.SetPixelColor(5, color);
	delay(1);
	strip.Show();
}

void Hardware::setAllLEDsRainbow(int bright)
{
	colorCounter++;
	if (colorCounter == 100)
		colorCounter = 0;
	phaseColor = colorCounter / 100.0 * 2 * PI;
	strip.SetBrightness(bright);
	strip.SetPixelColor(0, ledPhaseColor(0));
	strip.SetPixelColor(1, ledPhaseColor(2 * PI / 6));
	strip.SetPixelColor(2, ledPhaseColor(2 * 2 * PI / 6));
	strip.SetPixelColor(3, ledPhaseColor(2 * 3 * PI / 6));
	strip.SetPixelColor(4, ledPhaseColor(2 * 4 * PI / 6));
	strip.SetPixelColor(5, ledPhaseColor(2 * 5 * PI / 6));
	delay(1);
	strip.Show();
}

RgbColor Hardware::ledPhaseColor(float phase)
{
	return RgbColor((int)((sin(phaseColor + phase) / 2.0 + 0.5) * 255), (int)((sin(phaseColor + phase + 2 / 3 * PI) / 2.0 + 0.5) * 255), (int)((sin(phaseColor + phase + 4 / 3 * PI) / 2.0 + 0.5) * 255));
}