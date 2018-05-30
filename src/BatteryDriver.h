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


#ifndef BATTERYDRIVER_H
#define BATTERYDRIVER_H

#define BATTERY_STATUS_PIN 35 //voltage is scaled to fit 3V max - see documentation
#define BATTERY_PIN_CALIBRATION_REF_V 3.691 //referent voltage to calibrate battery pin

#define BATTERY_LOW_V 3.55
#define BATTERY_MID1_V 3.65
#define BATTERY_MID2_V 3.75
#define BATTERY_HIGH_V 3.85

#define BATTERY_EMPTY_P 0
#define BATTERY_LOW_P 15
#define BATTERY_MID_P 50
#define BATTERY_HIGH_P 100

#define BATTERY_HYSTERESIS 0.1

#include "Arduino.h"
#include "SharedData.h"

#include <Preferences.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

class BatteryDriver
{
public:
	BatteryDriver();
	void checkState();
	float readBatteryVoltage();
	float LPFvoltage(float valueNew);
	void calibrateBatteryPin();

	Preferences preferences;

	float batteryPinCalibrationValue = 0;
	uint16_t timeStarted = 0;
};
#endif
