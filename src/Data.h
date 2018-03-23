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


#ifndef GATA_H
#define GATA_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Data {
public:
	Data();

};

//control parameters:
struct Ctrl
{
	float joy1u[2] = { 0, 0 }; //r, fi
	float ax1u[2] = { 0, 0 };
	float joy2u[2] = { 0, 0 }; //r, fi
	float ax2u[2] = { 0, 0 };
	//float roboHightu = 4;
	int gaitID = 5;
	boolean buttons[2] = { 0, 0 }; // up, down, full rotation, part rotation (moving) 
	float trXYu[2] = { 0, 0 }; //robot tilt
	float stepHight = 2;

	bool linMode = 0; //state, if robot is in calibration mode
	bool running = 1; //state, if the robot is ready for control (power on-off)

	int nMove = 0; //how many times will current command execute (0 = home)
	int nMoveMax = 100; //max number nMove (watchdog timer)
	double tr[6] = { 6, 0, 0, 0, 0, 0 }; //initial translation and rotation vector
};

//parameters TODO: try both initialisations
struct Parameters
{
	const double a[3] = { 1.11,4.82,6.04 }; //dimensions of one leg
	const double dim[3] = { 3, 5.4, 7.2 }; //coordinates of the robots hips
	const double freq = 100; //frequency of the algorithm
	const double ts = 1.0 / freq;
};

#endif
