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


#ifndef ROBOTENGINE_H
#define ROBOTENGINE_H

#include "SharedData.h"

class RobotEngine
{
public:
	RobotEngine();
	void checkState();
	void modesGO();
	void batteryCheck();
	void calibrateBattery(uint8_t touchID, int8_t exitMode);

	//calibration mode variables
	int calibrationLegSelected = 0, calibrationLegSelectedCounter = 0, calibrationServoLayerSelected = 0;// , calibrationValue = 0;
	uint8_t calibrationLegSelectedMap[6] = { 0, 1, 2, 5, 4, 3 };
	Color calibrationServoLayerColors[3] = {RED, BLUE, YELLOW};
	uint8_t batteryCalibrationTouchPassword[5] = { TOUCH_0XX, TOUCH_XX0, TOUCH_0XX, TOUCH_XX0, TOUCH_0XX};
	uint8_t batteryCalibrationTouchPasswordCounter = 0;
	bool showcaseMove = 0;
	uint showcaseMoveTime = 0;
};
#endif
