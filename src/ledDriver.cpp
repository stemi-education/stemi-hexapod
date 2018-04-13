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
	strip.SetBrightness(50);
	strip.SetPixelColor(0, RgbColor(255, 0, 0));
	strip.SetPixelColor(1, RgbColor(100, 100, 0));
	strip.SetPixelColor(2, RgbColor(0, 255, 0));
	strip.SetPixelColor(3, RgbColor(0, 100, 100));
	strip.SetPixelColor(4, RgbColor(0, 0, 255));
	strip.SetPixelColor(5, RgbColor(100, 0, 100));
	delay(1);
	strip.Show();
};
