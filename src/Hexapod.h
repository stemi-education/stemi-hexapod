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


#ifndef HEXAPOD_H
#define HEXAPOD_H

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "SharedData.h"
#include "ServoDriver.h"
#include "BatteryDriver.h"
#include "LedDriver.h"
#include "Body.h"
#include "RobotEngine.h"
#include "BluetoothLowEnergy.h"
#include "TouchDriver.h"
#include "Dance.h"
#include "Server.h"
#include "Serial.h"

#include "ProductionVersion.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class Hexapod
{
public:
	Hexapod();
	void init(uint8_t mode = ROBOT_STANDBY_MODE);
};

#endif

