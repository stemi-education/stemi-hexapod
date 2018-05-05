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


#include "Dance.h"


Dance::Dance()
{

}

void Dance::setRobot(uint j)
{
	j = j<920 ? j : 919;
	int i = j/2;
	//if (i > 427) 
	i = i%460;
	
	
	//height
	if (heightArray[i] == HC)
		robot.danceInputData.translationZ = heights[HL] +(j % 2)*(heights[HM] - heights[HL]);
	else
		robot.danceInputData.translationZ = heights[heightArray[i]];

	//move
	robot.danceInputData.linearVelocity = movements[movesArray[i]].linearVelocity;
	robot.danceInputData.direction = movements[movesArray[i]].direction;
	robot.danceInputData.angularVelocity = movements[movesArray[i]].angularVelocity;

	//pose
	robot.danceInputData.translationX = poses[poseArray[i]].translationX;
	robot.danceInputData.translationY = poses[poseArray[i]].translationY; 
	robot.danceInputData.rotationX = poses[poseArray[i]].rotationX;
	robot.danceInputData.rotationY = poses[poseArray[i]].rotationY;
	robot.danceInputData.rotationZ = poses[poseArray[i]].rotationZ;
	
	//led
	robot.danceInputData.ledPrimarClr[0] = ledColors[ledColorArray[i]].ledPrimarClr[0];
	robot.danceInputData.ledPrimarClr[1] = ledColors[ledColorArray[i]].ledPrimarClr[1];
	robot.danceInputData.ledPrimarClr[2] = ledColors[ledColorArray[i]].ledPrimarClr[2];

	robot.danceInputData.ledSecondarClr[0] = ledColors[ledColorArray[i]].ledSecondarClr[0];
	robot.danceInputData.ledSecondarClr[1] = ledColors[ledColorArray[i]].ledSecondarClr[1];
	robot.danceInputData.ledSecondarClr[2] = ledColors[ledColorArray[i]].ledSecondarClr[2];

	robot.danceInputData.ledDiretion = ledColors[ledColorArray[i]].ledDiretion;
	robot.danceInputData.ledSpreadRatio = ledColors[ledColorArray[i]].ledSpreadRatio;

	robot.danceInputData.ledRotationSpeed = ledAnimations[ledAnimationArray[i]].ledRotationSpeed;
	robot.danceInputData.ledBlinkingSpeed = ledAnimations[ledAnimationArray[i]].ledBlinkingSpeed;

	Serial.print(i);
	Serial.print(": ");
	Serial.println(robot.danceInputData.ledBlinkingSpeed);
}
void Dance::tickStart()
{
	delay(5000);
	robot.danceInputData.ledPrimarClr[0] = ledColors[CR].ledPrimarClr[0];
	robot.danceInputData.ledPrimarClr[1] = ledColors[CR].ledPrimarClr[1];
	robot.danceInputData.ledPrimarClr[2] = ledColors[CR].ledPrimarClr[2];
	robot.danceInputData.ledDiretion = ledColors[CR].ledDiretion;
	robot.danceInputData.ledSpreadRatio = ledColors[CR].ledSpreadRatio;
	Serial.println(robot.ledCtrl.primarClr[0]);

	delay(549);
	robot.danceInputData.ledPrimarClr[0] = ledColors[CB].ledPrimarClr[0];
	robot.danceInputData.ledPrimarClr[1] = ledColors[CB].ledPrimarClr[1];
	robot.danceInputData.ledPrimarClr[2] = ledColors[CB].ledPrimarClr[2];
	robot.danceInputData.ledDiretion = ledColors[CB].ledDiretion;
	robot.danceInputData.ledSpreadRatio = ledColors[CB].ledSpreadRatio;
	Serial.println(robot.ledCtrl.primarClr[0]);

	delay(549);
	robot.danceInputData.ledPrimarClr[0] = ledColors[CY].ledPrimarClr[0];
	robot.danceInputData.ledPrimarClr[1] = ledColors[CY].ledPrimarClr[1];
	robot.danceInputData.ledPrimarClr[2] = ledColors[CY].ledPrimarClr[2];
	robot.danceInputData.ledDiretion = ledColors[CY].ledDiretion;
	robot.danceInputData.ledSpreadRatio = ledColors[CY].ledSpreadRatio;
	Serial.println(robot.ledCtrl.primarClr[0]);

	delay(549);
	robot.danceInputData.ledPrimarClr[0] = ledColors[CG].ledPrimarClr[0];
	robot.danceInputData.ledPrimarClr[1] = ledColors[CG].ledPrimarClr[1];
	robot.danceInputData.ledPrimarClr[2] = ledColors[CG].ledPrimarClr[2];
	robot.danceInputData.ledDiretion = ledColors[CG].ledDiretion;
	robot.danceInputData.ledSpreadRatio = ledColors[CG].ledSpreadRatio;
	Serial.println(robot.ledCtrl.primarClr[0]);
	delay(550);

}