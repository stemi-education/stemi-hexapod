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

Grip::Grip(Ctrl & ctrlNew, Parameters & parametersNew)
{
	parameters = &parametersNew;
	ctrl = &ctrlNew;
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
	float translationAmplitude = 1;
	ctrl->tr[1] = translationAmplitude * cos(-gaitPhi + 5 * PI / 4);
	ctrl->tr[2] = translationAmplitude * sin(-gaitPhi + 5 * PI / 4);
}
