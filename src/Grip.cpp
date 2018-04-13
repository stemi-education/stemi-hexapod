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


#include "Grip.h"

Grip::Grip(SharedData *sharedDataNew, Body &bodyNew)
{
	body = &bodyNew;
	sharedData = sharedDataNew;
}

void Grip::setGripParam(float pointCenterNew[3], float interspaceNew, float angleNew)
{
	pointCenter[0] = pointCenterNew[0];
	pointCenter[1] = pointCenterNew[1];
	pointCenter[2] = pointCenterNew[2];
	interspace = interspaceNew;
	angle= angleNew;
}

void Grip::calcPoints(){
	//x,y,z point 0
	point0[0] = pointCenter[0] + interspace / 2 * cos(angle + PI);
	point0[1] = pointCenter[1] + interspace / 2 * sin(angle + PI);
	point0[2] = pointCenter[2];

	//x,y,z point 1
	point1[0] = pointCenter[0] + interspace / 2 * cos(angle);
	point1[1] = pointCenter[1] + interspace / 2 * sin(angle);
	point1[2] = pointCenter[2];
}

void Grip::setPose(float gaitPhi)
{

	float poseVector[] = { 0,0 };
	uint8_t nGroundedLegs = 0;
	float poseVectorFactor = 0.4;

	for (int i = 0; i < body->nWalkingLegs; i++)
	{
		if (body->legs[body->walkingLegsMap[i]].gaitState)
		{
			nGroundedLegs++;
			//sum legs coordinates
			poseVector[0] += body->legs[body->walkingLegsMap[i]].c[0]; //x
			poseVector[1] += body->legs[body->walkingLegsMap[i]].c[1]; //y

			Serial.print(body->legs[body->walkingLegsMap[i]].label);
			Serial.print(" ");
			Serial.print(body->legs[body->walkingLegsMap[i]].c[0]);
			Serial.print(" ");
			Serial.print(body->legs[body->walkingLegsMap[i]].c[1]);
			Serial.print(" ");
		}
		Serial.print(" | ");
	}
	poseVector[0] /= nGroundedLegs;
	poseVector[1] /= nGroundedLegs;

	Serial.print("PoseVector: ");
	Serial.print(poseVector[0]);
	Serial.print(" ");
	Serial.println(poseVector[1]);

	sharedData->moveCtrl.poseVector[1] = poseVector[0]* poseVectorFactor;
	sharedData->moveCtrl.poseVector[2] = poseVector[1] * poseVectorFactor-2;
}

void Grip::setLegWorkspace()
{
	body->legs[1].setCustomWs(0, 3, 0.7, 0.6);
	body->legs[4].setCustomWs(0, 3, 0.7, 0.6);

	body->legs[2].setCustomWs(3, 3, 1, 1);
	body->legs[5].setCustomWs(-3, 3, 1, 1);
}

void Grip::resetWorkspace()
{
	body->legs[1].setCustomWs(0, 0, 1, 1);
	body->legs[4].setCustomWs(0, 0, 1, 1);

	body->legs[2].setCustomWs(0, 0, 1, 1);
	body->legs[5].setCustomWs(0, 0, 1, 1);
}
