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


#include "Robot.h"

void setup() 
{
	Serial.begin(115200);
	Serial1.begin(115200);
}

void loop()
{
	Robot robot; // initalise the robot
  
	robot.wakeUp(); //wake up the robot

	while(1) //repeat following commands forever
	{
		robot.goHome();
		robot.goForward(10);
		robot.goHome();
		robot.goBackwards(10);
		robot.goHome();
		robot.turnLeft(90);
		robot.goHome();
		robot.turnRight(90);

		while (1)
		{
			robot.goHome();
		}
	}
}
