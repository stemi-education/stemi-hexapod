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

#include "Leg.h"
#include "CircleIntersect.h"
#include <cmath>

static double absolute(double number)
{
	return number >= 0 ? number : -number;
}

Leg::Leg()
{
}

void Leg::init(char* labelNew, double offNew, double adNew[2], double aNew[3], double trNew[6], double freqNew)
{
	terrainMode = 0;
	groundLevel = 0;
	earlyFootholdGroundedFlag = 0;
	groundingState = 0;
	freeMode = 0;

	double cNew[3];

	freq = freqNew;
	ts = 1.0 / freq;

	strcpy(label, labelNew);

	cMat[15] = 1;

	off = offNew;
	ad[0] = adNew[0]; ad[1] = adNew[1];
	a[0] = aNew[0]; a[1] = aNew[1]; a[2] = aNew[2];
	setTr(trNew);
	setWs(0.85);
	wsCenter[2] = 0; //just once, in initialization so c could have z=0 component
	setC(wsCenter);

	stepHight = 4.0;
	preFootholdHight = stepHight;
	preFootholdScaler = 1.3;

	alpha_rise_move = 0.85;
	alpha_lower_move = 0.1;
	alpha_ground_move = 0.95;

	actualState = 0;
	homeMark = 1;
}

void Leg::printc()
{
	Serial.print(label);
	Serial.print(" c= ");
	Serial.print(c[0]);
	Serial.print(" ");
	Serial.print(c[1]);
	Serial.print(" ");
	Serial.println(c[2]);
}

void Leg::printq()
{
	Serial.print(label);
	Serial.print(" q= ");
	Serial.print(q[0]);
	Serial.print(" ");
	Serial.print(q[1]);
	Serial.print(" ");
	Serial.println(q[2]);
}

float Leg::saturate(float value, float minimum, float maximum)
{
	return min(maximum, max(minimum, value));
}

double wrapToPi(double r)
{ //TODO maybe provide more robust solution
	while (r > PI) {
		r -= 2 * PI;
	}
	while (r <= -PI) {
		r += 2 * PI;
	}
	return r;
}

double wrapTo2Pi(double r)
{ //TODO mozda je moguce robusnije rijesit ovo
	while (r > 2 * PI) {
		r -= 2 * PI;
	}
	while (r <= 0.0) {
		r += 2 * PI;
	}
	return r;
}


void Leg::IK()
{/*
	cMatInv=Tinv*cMat;
	q[0]=wrapToPi (    atan2(cMatInv(1,3),cMatInv(0,3))-off);
		q[1]=wrapToPi (     acos(min(max(((pow(a[1],2) - pow(a[2],2) + pow(cMatInv(2,3),2) + pow((a[0] - pow((pow(cMatInv(0,3),2) + pow(cMatInv(1,3),2)),(1.0/2))),2))/(2*a[1]*pow((pow(cMatInv(2,3),2) + pow(a[0] - pow((pow(cMatInv(0,3),2) + pow(cMatInv(1,3),2)),1.0/2),2)),1.0/2))), -1.0), 1.0)) + atan2(cMatInv(2,3), pow((pow(cMatInv(0,3),2) + pow(cMatInv(1,3),2)),1.0/2) - a[0]));
		q[2]=wrapToPi (     -PI + acos(min(max(((pow(a[1],2) + pow(a[2],2) - pow(cMatInv(2,3),2) - pow((a[0] - pow((pow(cMatInv(0,3),2) + pow(cMatInv(1,3),2)),1.0/2)),2))/(2*a[1]*a[2])), -1.0), 1.0)));
*/
	Matrix.Multiply(Tinv, cMat, 4, 4, 4, cMatInv);
	q[0] = wrapToPi(atan2(cMatInv[7], cMatInv[3]) - off);
	q[1] = wrapToPi(acos(min(max(((pow(a[1], 2) - pow(a[2], 2) + pow(cMatInv[11], 2) + pow((a[0] - pow((pow(cMatInv[3], 2) + pow(cMatInv[7], 2)), (1.0 / 2))), 2)) / (2 * a[1] * pow((pow(cMatInv[11], 2) + pow(a[0] - pow((pow(cMatInv[3], 2) + pow(cMatInv[7], 2)), 1.0 / 2), 2)), 1.0 / 2))), -1.0), 1.0)) + atan2(cMatInv[11], pow((pow(cMatInv[3], 2) + pow(cMatInv[7], 2)), 1.0 / 2) - a[0]));
	q[2] = wrapToPi(-PI + acos(min(max(((pow(a[1], 2) + pow(a[2], 2) - pow(cMatInv[11], 2) - pow((a[0] - pow((pow(cMatInv[3], 2) + pow(cMatInv[7], 2)), 1.0 / 2)), 2)) / (2 * a[1] * a[2])), -1.0), 1.0)));

	q[0] = saturate(q[0], -PI / 2, PI / 2);
	q[1] = saturate(q[1], -PI / 2, PI / 2);
	q[2] = saturate(q[2], -PI, 0);
}

void Leg::setC(double cNew[3])
{
	cMat[3] = cNew[0]; cMat[7] = cNew[1]; cMat[11] = cNew[2];
	c[0] = cNew[0]; c[1] = cNew[1]; c[2] = cNew[2];
}

void Leg::setTr(double trNew[6])
{
	tr[0] = trNew[0]; tr[1] = trNew[1]; tr[2] = trNew[2]; tr[3] = trNew[3]; tr[4] = trNew[4]; tr[5] = trNew[5];

	T[0] = sin(PI / 2 + tr[3])*sin(PI / 2 + tr[5]) + cos(PI / 2 + tr[3])*cos(PI / 2 + tr[4])*cos(PI / 2 + tr[5]);
	T[1] = cos(PI / 2 + tr[3])*sin(PI / 2 + tr[4]);
	T[2] = cos(PI / 2 + tr[3])*cos(PI / 2 + tr[4])*sin(PI / 2 + tr[5]) - cos(PI / 2 + tr[5])*sin(PI / 2 + tr[3]);
	T[3] = tr[1] + ad[1] * cos(PI / 2 + tr[3])*sin(PI / 2 + tr[4]) + ad[0] * sin(PI / 2 + tr[3])*sin(PI / 2 + tr[5]) + ad[0] * cos(PI / 2 + tr[3])*cos(PI / 2 + tr[4])*cos(PI / 2 + tr[5]);

	T[4] = cos(PI / 2 + tr[4])*cos(PI / 2 + tr[5])*sin(PI / 2 + tr[3]) - cos(PI / 2 + tr[3])*sin(PI / 2 + tr[5]);
	T[5] = sin(PI / 2 + tr[3])*sin(PI / 2 + tr[4]);
	T[6] = cos(PI / 2 + tr[3])*cos(PI / 2 + tr[5]) + cos(PI / 2 + tr[4])*sin(PI / 2 + tr[3])*sin(PI / 2 + tr[5]);
	T[7] = tr[2] - ad[0] * cos(PI / 2 + tr[3])*sin(PI / 2 + tr[5]) + ad[1] * sin(PI / 2 + tr[3])*sin(PI / 2 + tr[4]) + ad[0] * cos(PI / 2 + tr[4])*cos(PI / 2 + tr[5])*sin(PI / 2 + tr[3]);

	T[8] = cos(PI / 2 + tr[5])*sin(PI / 2 + tr[4]);
	T[9] = -cos(PI / 2 + tr[4]);
	T[10] = sin(PI / 2 + tr[4])*sin(PI / 2 + tr[5]);
	T[11] = tr[0] - ad[1] * cos(PI / 2 + tr[4]) + ad[0] * cos(PI / 2 + tr[5])*sin(PI / 2 + tr[4]);

	T[12] = 0;
	T[13] = 0;
	T[14] = 0;
	T[15] = 1;

	Matrix.Copy(T, 4, 4, Tinv); //copy the matrix before calculatin the inverse
	Matrix.Invert(Tinv, 4); //inversion of the matrix
}

void Leg::setQ(double qNew[3])
{
	q[0] = qNew[0]; q[1] = qNew[1]; q[2] = qNew[2];
}

void Leg::setWs(double wsScalar)
{
	double alpha = asin(tr[0] / (a[1] + a[2]));//PI/6.0; // worst case angle of the leg. to calculate workspace (see documentation)
	double initDist = a[0] + (a[1] + a[2])*cos(alpha) / 2.0; //distance from the hip (starting coordinates of the leg - for home position) (added a[0] to avoid collision)
	//leg coordinates (end of the leg)
	wsCenter[0] = initDist*cos(off)*wsScalar + ad[0];
	wsCenter[1] = initDist*sin(off)*wsScalar + ad[1];

	//workspace circle
	wsRadius = (a[1] + a[2])*cos(alpha) / 2.0*0.5;// last number reduces the ideal circle radius to make walking more stable
	//now, the workspace is defined with the wsCenter and wsRadius for this leg
}

void Leg::setCustomWs(float xOffset, float yOffset, double wsScalar, float wsRadiusScalar)
{
	double alpha = asin(tr[0] / (a[1] + a[2]));//PI/6.0; // worst case angle of the leg. to calculate workspace (see documentation)
	double initDist = a[0] + (a[1] + a[2])*cos(alpha) / 2.0; //distance from the hip (starting coordinates of the leg - for home position) (added a[0] to avoid collision)
																													 //leg coordinates (end of the leg)
	wsCenter[0] = initDist * cos(off)*wsScalar + ad[0] + xOffset;
	wsCenter[1] = initDist * sin(off)*wsScalar + ad[1] + yOffset;

	//workspace circle
	wsRadius = (a[1] + a[2])*cos(alpha) / 2.0*0.5*wsRadiusScalar;// last number reduces the ideal circle radius to make walking more stable
																								//now, the workspace is defined with the wsCenter and wsRadius for this leg

	homeMark = 0;
}



void Leg::setMoveParam(double moveCenterNew[2], double moveDeltaFiNew) {
	//moveDeltaFi = moveDelta / r ... [r = sqrt(  pow(moveCenter[0],2) + pow(moveCenter[1],2)  )]
	moveCenter[0] = moveCenterNew[0];
	moveCenter[1] = moveCenterNew[1];
	moveDeltaFi = moveDeltaFiNew;
}

void Leg::setHomeParam(double moveDeltaNew) {
	moveDelta = -moveDeltaNew;
}
void Leg::setStepHight(double stepHightNew) {
	stepHight = stepHightNew;
	preFootholdHight = stepHight;
}

void Leg::calcWsRange()
{
	//calculate fi0, fiCur, fiPlus, fiMinus for current moveCenter
	fiCur = calcFi(moveCenter, c);
	rCur = sqrt(pow(moveCenter[0] - c[0], 2) + pow(moveCenter[1] - c[1], 2));

	rFoothold = sqrt(pow(moveCenter[0] - wsCenter[0], 2) + pow(moveCenter[1] - wsCenter[1], 2));

	// radius and center of circles
	Circle wsCircle(wsRadius, wsCenter);
	Circle moveCircle(rCur, moveCenter);
	Circle wsToMoveCircle(rFoothold, moveCenter);
	Point2d i1, i2, i3, i4;

	wsCircle.intersect(moveCircle, i1, i2);

	double 	intersection1[2] = { i1.x(), i1.y() },
		intersection2[2] = { i2.x(), i2.y() };


	double fi1 = wrapTo2Pi(calcFi(moveCenter, intersection1));
	double fi2 = wrapTo2Pi(calcFi(moveCenter, intersection2));
	double fiMin = min(fi1, fi2);
	double fiMax = max(fi1, fi2);

	if (moveDeltaFi >= 0)
		if (rCur <= wsRadius)
		{
			fiMinus = -100;	// just so it will never be a limitating factor while walking
			fiPlus = 100;		// this leg doesnt affect gait timing anyomore 
						//(how much workspace it has left) - has the largest moveDeltaFi
		}
		else
			if ((fiMax - fiMin) < PI)
			{
				fiMinus = wrapToPi(fiMin - fiCur);
				fiPlus = wrapToPi(fiMax - fiCur);
			}
			else
			{
				fiMinus = wrapToPi(fiMax - fiCur);
				fiPlus = wrapToPi(fiMin - fiCur);
			}
	else
		if (rCur <= wsRadius)
		{
			fiMinus = 100;       // just so it will never be a limitating factor while walking
			fiPlus = -100;		// this leg doesnt affect gait timing anyomore 
						//(how much workspace it has left) - has the largest moveDeltaFi
		}
		else
			if ((fiMax - fiMin) < PI)
			{
				fiMinus = wrapToPi(fiMax - fiCur);
				fiPlus = wrapToPi(fiMin - fiCur);
			}
			else
			{
				fiMinus = wrapToPi(fiMin - fiCur);
				fiPlus = wrapToPi(fiMax - fiCur);
			};

	wsCircle.intersect(wsToMoveCircle, i3, i4);
	double 	intersection3[2] = { i3.x(), i3.y() },
		intersection4[2] = { i4.x(), i4.y() };

	double fi3 = wrapTo2Pi(calcFi(moveCenter, intersection3));
	double fi4 = wrapTo2Pi(calcFi(moveCenter, intersection4));
	fiMin = min(fi3, fi4);
	fiMax = max(fi3, fi4);

	if (rFoothold <= wsRadius)
	{
		fiFoothold = calcFi(moveCenter, wsCenter);
	}
	else
		if (moveDeltaFi >= 0)
			if ((fiMax - fiMin) < PI)
			{
				fiFoothold = fiMin;
			}
			else
			{
				fiFoothold = fiMax;
			}
		else
			if ((fiMax - fiMin) < PI)
			{
				fiFoothold = fiMax;
			}
			else
			{
				fiFoothold = fiMin;
			}

	cFoothold[0] = rFoothold*cos(fiFoothold) + moveCenter[0];
	cFoothold[1] = rFoothold*sin(fiFoothold) + moveCenter[1];
	pathToFoothold = sqrt(pow(c[0] - cFoothold[0], 2) + pow(c[1] - cFoothold[1], 2));
}

void Leg::calcHomeStepRange()
{
	cFoothold[0] = wsCenter[0];
	cFoothold[1] = wsCenter[1];
	pathToFoothold = sqrt(pow(c[0] - cFoothold[0], 2) + pow(c[1] - cFoothold[1], 2));
}

double Leg::calcFi(double center[2], double point[2])
{
	return wrapTo2Pi(atan2((double)(point[1] - center[1]), (double)(point[0] - center[0])));
}

// ***********************GAIT parametri******************** 

void Leg::setGaitUpDown(double gaitUpFiNew, double gaitDownFiNew)
{
	gaitUpFi = wrapTo2Pi(gaitUpFiNew);
	gaitUpFiIdeal = gaitUpFi;
	gaitDownFi = wrapTo2Pi(gaitDownFiNew);
	gaitDownFiIdeal = gaitDownFi;
	gaitUpTime = wrapTo2Pi(gaitDownFi - gaitUpFi);
	gaitDownTime = wrapTo2Pi(gaitUpFi - gaitDownFi);
}

void Leg::setGaitCurFi(double gaitCurFiNew)
{
	gaitCurFi = wrapTo2Pi(gaitCurFiNew);
}

void Leg::checkGaitState()
{
	if (groundingState && actualState)
	{
		earlyFootholdGround();
	}
	if (gaitDownFi > gaitUpFi)
		if ((gaitCurFi >= gaitUpFi) && (gaitCurFi < gaitDownFi))
			gaitState = 0;
		else
			gaitState = 1;
	else if ((gaitCurFi >= gaitDownFi) && (gaitCurFi < gaitUpFi))
		gaitState = 1;
	else
		gaitState = 0;

	if (!groundingState && !gaitState)
	{
		//reset everything to default values
		setGaitUpDown(gaitUpFiIdeal, gaitDownFiIdeal);// if the leg is in the air, reset the modified gaitup/down phase 
		groundLevel = 0;
		earlyFootholdGroundedFlag = 0;
	}
	//check how long until the gait state change
	gaitNext = min(wrapTo2Pi(gaitDownFi - gaitCurFi), wrapTo2Pi(gaitUpFi - gaitCurFi));
	if (gaitNext == 0)
	{ //just in case curFi is identical to Up/Down, go to the next state and nextFi
		gaitNext = max(wrapTo2Pi(gaitDownFi - gaitCurFi), wrapTo2Pi(gaitUpFi - gaitCurFi));
	}
}

void Leg::incGaitCurFi(double gaitDeltaFi)
{
	gaitCurFi = wrapTo2Pi(gaitCurFi + gaitDeltaFi);
}

// ***********************GAIT algoritam******************** 
void Leg::calcStepScale()
{
	if (gaitState)
		stepScalePlus = absolute(fiPlus / gaitNext);
	else
		stepScalePlus = absolute((pathToFoothold) / rCur / gaitNext*gaitUpTime / gaitDownTime);
}

void Leg::calcHomeStepScale()
{
	if (groundingState)
		stepScalePlus = absolute((pathToFoothold + c[2] * preFootholdScaler) / gaitNext);
	else
		stepScalePlus = absolute((pathToFoothold + stepHight*preFootholdScaler) / gaitNext);
}

void Leg::calcAll()
{
	checkGaitState();
	calcWsRange();
	calcStepScale();
}

void Leg::calcHomeAll() {
	checkGaitState();
	calcHomeStepRange();
	calcHomeStepScale();
}

void Leg::move(double gaitDeltaFi, double slowingScale) {
	homeMark = 0; //your foot is no longer in the home position
	double stepsToFoothold, angleToFoothold;
	double cNew[3];

	cNew[2] = c[2];
	if (gaitState)
	{
		//  stancing while following circular trajectory
		fiCur += moveDeltaFi*slowingScale; //reduce the angular increment - slow down for the amount dictated by clowingScale (due to too fast leg movement of one leg)
		cNew[0] = rCur*cos(fiCur) + moveCenter[0];
		cNew[1] = rCur*sin(fiCur) + moveCenter[1];
		cNew[2] = ground(cNew[2]);
		setC(cNew);
		groundingState = 0;
	}
	else

	{ //leg is traveling in air - swinging
		stepsToFoothold = gaitNext / gaitDeltaFi;
		if (pathToFoothold > (pathToFoothold + preFootholdHight*preFootholdScaler) / stepsToFoothold)
		{
			//swinging while following straight line to foothold as a trajectory (in x-y perspective)
			angleToFoothold = atan2(cFoothold[1] - c[1], cFoothold[0] - c[0]);
			cNew[0] = c[0] + (pathToFoothold + preFootholdHight*preFootholdScaler)*cos(angleToFoothold) / ceil(stepsToFoothold); //treba doci stepsBeforeLanding koraka prije u koordinate footholda da ima vise vremena za spustanje
			cNew[1] = c[1] + (pathToFoothold + preFootholdHight*preFootholdScaler)*sin(angleToFoothold) / ceil(stepsToFoothold);
			cNew[2] = alpha_rise_move*cNew[2] + (1 - alpha_rise_move)*stepHight; //ako nisi blizu kraja guranja dizi nogu po PT1 clanu
			groundingState = 0;
		}
		else
		{
			//swinging while following straight line to foothold as a trajectory (in z perspective) - lowering
			cNew[0] = cFoothold[0];
			cNew[1] = cFoothold[1];
			cNew[2] = cNew[2] + (groundLevel - cNew[2]) / ceil(stepsToFoothold);
			groundingState = 1;
		}
		setC(cNew);

	}

}

bool Leg::checkHomeMark() {
	return homeMark;
}
void Leg::home(double gaitDeltaFi) {
	double stepsToFoothold, angleToFoothold;
	double cNew[3];
	cNew[2] = c[2];
	double moveDeltaScaled = absolute(gaitDeltaFi*stepScalePlus);

	if (!homeMark && !gaitState)
	{ //swinging while following straight line to foothold as a trajectory (in x-y perspective)
		stepsToFoothold = gaitNext / gaitDeltaFi;
		if (pathToFoothold > (pathToFoothold + preFootholdHight*preFootholdScaler) / stepsToFoothold)
		{
			angleToFoothold = atan2(cFoothold[1] - c[1], cFoothold[0] - c[0]);
			cNew[0] = c[0] + (pathToFoothold + preFootholdHight*preFootholdScaler)*cos(angleToFoothold) / ceil(stepsToFoothold); //treba doci stepsBeforeLanding koraka prije u koordinate footholda da ima vise vremena za spustanje
			cNew[1] = c[1] + (pathToFoothold + preFootholdHight*preFootholdScaler)*sin(angleToFoothold) / ceil(stepsToFoothold);

			cNew[2] = alpha_rise_move*cNew[2] + (1 - alpha_rise_move)*stepHight; //ako nisi blizu kraja guranja dizi nogu po PT1 clanu
			groundingState = 0;
		}
		else
		{
			if (pathToFoothold < 0.2 && absolute(c[2] - groundLevel) < 0.2)
			{ // in case gaistate = 1 and distance to foothold is shorter than moveDelta
				homeMark = 1;
				cNew[0] = cFoothold[0]; //move the leg to the home position and mark it as homed
				cNew[1] = cFoothold[1];
				cNew[2] = groundLevel;
				setC(cNew);
				earlyFootholdGround();
			}
			else
			{
				cNew[0] = cFoothold[0];
				cNew[1] = cFoothold[1];
				cNew[2] = cNew[2] + (groundLevel - cNew[2]) / ceil(stepsToFoothold);
				groundingState = 1;
			}
		}
		setC(cNew);
	}
	else if (gaitState && pathToFoothold < 0.2 && absolute(c[2] - groundLevel) < 0.2)
	{
		homeMark = 1; // in case gaistate = 1 and distance to foothold is shorter than moveDelta
		cNew[0] = cFoothold[0]; //move the leg to the home position and mark it as homed
		cNew[1] = cFoothold[1];
		cNew[2] = groundLevel;
		setC(cNew);
	}
}
void Leg::earlyFootholdGround()
{
	groundLevel = c[2]; //new ground level (current level) since it has unexpectedly urrived to the ground earlier
	gaitDownFi = gaitCurFi; //current gait phase is temporarily set to be the phase of landing the leg since it is earlyer on the ground.
	earlyFootholdGroundedFlag = 1;
}

//grounding the leg
double Leg::ground(double zAxis) {
	zAxis = alpha_ground_move*zAxis + (1 - alpha_ground_move)*groundLevel;
	return zAxis;
}

//ground the leg and refresh cMat
void Leg::setCground() {
	c[2] = alpha_ground_move*c[2] + (1 - alpha_ground_move)*groundLevel;
	cMat[11] = c[2];
}



