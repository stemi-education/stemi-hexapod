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


#ifndef ROBOT_H
#define ROBOT_H

#define LIN_MODE_PERMANENT 0 //set this to 1 if you want your robot to stand still in linearisation pose permanently - all the servos in home position - used for robot assembly

#include "Hardware.h"
#include "Body.h"
#include "Data.h"

class Robot
{
public:
	Robot();
	void wakeUp();
	int go();
	Hardware hardware;
	Ctrl ctrl;
	Parameters parameters;
	Body body;

	void measureTime();
	void wait();

	/* High level functions that are ment to be called once. They set the parameters and then loop so
	* the robot will do some action like walk the given distance.
	*/
	void loopMove();
	void loopHome(float timeWaiting);
	void goHome(float time);

	void goForward(float distance);
	void goBackwards(float distance);
	void goLeft(float distance);
	void goRight(float distance);
	void turnLeft(float distance);
	void turnRight(float distance);

	void setRotation(float xTilt, float yTilt, float zTilt);
	void setTranslation(float xRotation, float yRotation, float zRotation);
	void danceHip(float angle, float translation, float time);

	void resetPose();

	long startTime;

	//high level functions
	float goSpeed = 5;
	float turnSpeed = 0.4;
};

#endif

