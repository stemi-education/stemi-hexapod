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

#include "Body.h"

static double absolute(double number)
{
	return number >= 0 ? number : -number;
}

Body::Body(Ctrl &ctrlNew, Parameters &parametersNew)
{
	parameters = &parametersNew;
	ctrl = &ctrlNew;

	ctrl->linMode = 0; //state, if the robot is ready for control
	ctrl->running = 1;


	ctrl->nMove = 0;
	ctrl->nMoveMax = 100;

	double ad[2], a[3], dim[3];

	ts = 1.0 / parameters->freq;

	trCurrent[0] = ctrl->tr[0];//0;//Start with legs rised up - simple standup routine
	trCurrent[1] = ctrl->tr[1];
	trCurrent[2] = ctrl->tr[2];
	trCurrent[3] = ctrl->tr[3];
	trCurrent[4] = ctrl->tr[4];
	trCurrent[5] = ctrl->tr[5];

	dim[0] = parameters->dim[0]; dim[1] = parameters->dim[1]; dim[2] = parameters->dim[2]; // body: [x1, x2, y]
	a[0] = parameters->a[0]; a[1] = parameters->a[1]; a[2] = parameters->a[2];

	//Legs initialisation
	ad[0] = dim[0]; ad[1] = dim[2];
	legs[0].init("R1", PI / 4, ad, a, trCurrent, parameters->freq);

	ad[0] = dim[1]; ad[1] = 0;
	legs[1].init("R2", 0.0, ad, a, trCurrent, parameters->freq);

	ad[0] = dim[0]; ad[1] = -dim[2];
	legs[2].init("R3", -PI / 4, ad, a, trCurrent, parameters->freq);

	ad[0] = -dim[0]; ad[1] = dim[2];
	legs[3].init("L1", 3.0*PI / 4, ad, a, trCurrent, parameters->freq);

	ad[0] = -dim[1]; ad[1] = 0;
	legs[4].init("L2", PI, ad, a, trCurrent, parameters->freq);

	ad[0] = -dim[0]; ad[1] = -dim[2];
	legs[5].init("L3", -3.0*PI / 4, ad, a, trCurrent, parameters->freq);

	legs[1].setCustomWs(0, 3, 0.7, 0.6);
	legs[4].setCustomWs(0, 3, 0.7, 0.6);

	legs[2].setCustomWs(3, 3, 1, 1);
	legs[5].setCustomWs(-3, 3, 1, 1);

	setGaitUpDown(gait.selectSequence(ctrl->gaitID));
	setGaitCurFi(gait.selectStart(ctrl->gaitID));

	setStepHight(ctrl->stepHight);

	alpha_tr = 0.9;

	setMoveParam(0, PI / 2, 0, 0);

	maxAllowedSpeed = 30;

}

float Body::saturate(float value, float minimum, float maximum)
{
	return minn(maximum, maxx(minimum, value));
}

void Body::IK() {
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].IK();
}

void Body::packQArray() //pack all the q's into one array
{
	for (int i = 0; i < nLegs*3; i++) qAll[i] = legs[i / 3].q[i % 3];
}

void Body::setLinMode()
{
	for (int i = 0; i < nLegs; i++) legs[i].setLinMode();
	packQArray();
}

void Body::setTr(double trNew[6]) {
	for (int i = 0; i < nLegs; i++) legs[i].setTr(trNew);
}

void Body::printq() {
	for (int i = 0; i < nLegs; i++) legs[i].printq();
}

void Body::printc() {
	for (int i = 0; i < nLegs; i++)
	{
		legs[i].printq();
	}
}

//-----------GAIT and WS-----------------

void Body::setWs(double wsScalar)
{
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setWs(wsScalar);
}

void Body::calcAll()
{
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].calcAll();
}

void Body::calcHomeAll()
{
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].calcHomeAll();
}

void Body::incGaitFi(double gaitStep)
{
	for (int i = 0; i < nWalkingLegs; i++)
	{
		legs[walkingLegsMap[i]].incGaitCurFi(gaitStep);
		gaitCurFi = legs[walkingLegsMap[i]].gaitCurFi;
	}
}

void Body::setGaitUpDown(double gaitArray[12])
{
	for (int i = 0; i < nWalkingLegs; i++)
	{
		legs[walkingLegsMap[i]].setGaitUpDown(gaitArray[walkingLegsMap[i] * 2] * PI / 3.0, gaitArray[walkingLegsMap[i] * 2 + 1] * PI / 3.0);
		//TODO zasto se upisuju krivi FIjevi? ispiši i quadWaveBase pa provjeri
	}
}

void Body::setGaitCurFi(double gaitCurFiNew)
{
	for (int i = 0; i < nWalkingLegs; i++)
	{
		legs[walkingLegsMap[i]].setGaitCurFi(gaitCurFiNew*PI / 3.0);
		gaitCurFi = legs[walkingLegsMap[i]].gaitCurFi;
	}
}

void Body::setMoveParam(double speedNew, double fiNew, double deltaFiNew, int nMoveNew) {
	speed = speedNew;
	double moveCenterNew[2];
	moveDeltaFi = deltaFiNew == 0 ? speed / parameters->freq / 100000 : deltaFiNew / parameters->freq;

	double r = deltaFiNew == 0 ? 100000 : absolute(speedNew) / deltaFiNew;
	moveCenterNew[0] = r*cos(fiNew - PI / 2);
	moveCenterNew[1] = r*sin(fiNew - PI / 2);

	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setMoveParam(moveCenterNew, moveDeltaFi); //TODO wrapper

	ctrl->nMove = nMoveNew;
}

void Body::setRotateParam(double  moveDeltaFiNew) {
	moveDeltaFi = moveDeltaFiNew;
	double moveCenterNew[2] = { 0, 0 };

	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setMoveParam(moveCenterNew, moveDeltaFi);
}

void Body::setHomeParam(double moveDeltaNew) {
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setHomeParam(moveDeltaNew);
}

void Body::setStepHight(double stepHightNew) {
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setStepHight(stepHightNew);
}

void Body::scaleStepFi() {
	double minScale = 9999.9;
	double maxCurSpeed = -9999.9;
	for (int i = 0; i < nWalkingLegs; i++)
		if (legs[walkingLegsMap[i]].gaitState)
			minScale = minn(minScale, legs[walkingLegsMap[i]].stepScalePlus);

	gaitDeltaFi = absolute(moveDeltaFi / minScale);
	slowingScale = 1;

	double preFootholdHight = ctrl->stepHight, preFootholdScaler = 1.3;

	for (int i = 0; i < nWalkingLegs; i++)
		if (!legs[walkingLegsMap[i]].gaitState)
			maxCurSpeed = maxx(maxCurSpeed, (legs[walkingLegsMap[i]].pathToFoothold + legs[walkingLegsMap[i]].c[2] * preFootholdScaler) / (legs[walkingLegsMap[i]].gaitNext / gaitDeltaFi*ts));

	if (maxCurSpeed > maxAllowedSpeed) //if leg speed is higher than allowed
	{
		//At least ne of the legs in the air is too fast - slowing down with slowingScale
		slowingScale = maxAllowedSpeed / maxCurSpeed;
		gaitDeltaFi = absolute(gaitDeltaFi*maxAllowedSpeed / maxCurSpeed);
	}
}

void Body::scaleHomeStep() {
	double maxScale = -9999.9;

	for (int i = 0; i < nWalkingLegs; i++)
		if (!legs[walkingLegsMap[i]].gaitState && !legs[walkingLegsMap[i]].checkHomeMark())
			maxScale = maxx(maxScale, legs[walkingLegsMap[i]].stepScalePlus);

	bool gateStateAll = 1;
	for (int i = 0; i < nWalkingLegs; i++)
		gateStateAll = gateStateAll && legs[walkingLegsMap[i]].gaitState;

	if (gateStateAll)
		gaitDeltaFi = 4 * ts; // scalar speeds up the period when all legs are grounded
	else

		if (maxScale > 0.0)
			gaitDeltaFi = absolute(8 / parameters->freq / maxScale); //scalar speeds up the legs traveling to the home position

}

void Body::move() {
	calcAll();
	scaleStepFi();


	if (gaitDeltaFi < 1.5)
	{
		for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].move(gaitDeltaFi, slowingScale);
		incGaitFi(gaitDeltaFi);
	}
	else
	{
		Serial.println("GG"); //gaitDeltaFi too high - wierd math happened - BLOCKING
	}
}

bool Body::checkHomeMark() {

	bool homeMarkAll = 1;
	for (int i = 0; i < nWalkingLegs; i++)
		homeMarkAll = homeMarkAll && legs[walkingLegsMap[i]].checkHomeMark();
	return homeMarkAll;
}

void Body::home(float moveDeltaNew) {
	if (!checkHomeMark())
	{
		setHomeParam(moveDeltaNew);
		calcHomeAll();
		scaleHomeStep();

		for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].home(gaitDeltaFi);

		incGaitFi(gaitDeltaFi);
	}
	else {
		//All legs are at home position
		setCground();
		for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].gaitState = 1;
		setGaitCurFi(gait.selectStart(ctrl->gaitID));
	}
}

void Body::run() {
	if (ctrl->running)
	{
		if (!ctrl->linMode)
		{
			if (ctrl->nMove > 0) ctrl->nMove--; //check the duration of the command and reduce nMove
			else
			{
				setMoveParam(0, PI / 2, 0, ctrl->nMoveMax); //if nMove == 0 go to home ... no command present
				ctrl->tr[0];
				//ctrl->tr[1] = 0;
				//ctrl->tr[2] = 0; STAVITI DA BUDE U CTRL
				//ctrl->tr[3] = 0;
				//ctrl->tr[4] = 0;
				//ctrl->tr[5] = 0;
			}

			//setWs(0.85);
			trPT1(); //smooth the tr tranistion
			setTr(trCurrent); //set the smoothed tr 




			if (speed || moveDeltaFi)
			{
				//Serial.println("M");
				move();
				//printc();
			}
			else
			{
				//Serial.println("H");
				home(0.001);
				//printc();
			}
			IK();
		}
		else
		{
			//Serial.println("L");
			setLinMode();
		}
	}
	else
	{
		setMoveParam(0, PI / 2, 0, ctrl->nMoveMax); //go to home, dont receive commands while !running
		ctrl->tr[0] = 0; //raise legs = put them on 0 hight
		ctrl->tr[1] = 0;
		ctrl->tr[2] = 0;
		ctrl->tr[3] = 0;
		ctrl->tr[4] = 0;
		ctrl->tr[5] = 0;

		//setWs(0.85);
		trPT1(); //smooth the tr tranistion
		setTr(trCurrent); //set the smoothed tr 

		home(0.001);
		IK();
	}
}

void Body::setCground() {
	for (int i = 0; i < nWalkingLegs; i++) legs[walkingLegsMap[i]].setCground();
}

float PT1(float input, float valuePrev, float alpha) {
	return alpha*valuePrev + (1 - alpha)*(input);
}
void Body::trPT1() {
	trCurrent[0] = PT1(ctrl->tr[0], trCurrent[0], alpha_tr - 0.02);
	trCurrent[1] = PT1(ctrl->tr[1], trCurrent[1], alpha_tr - 0.02);
	trCurrent[2] = PT1(ctrl->tr[2], trCurrent[2], alpha_tr - 0.02);
	trCurrent[3] = PT1(ctrl->tr[3], trCurrent[3], alpha_tr);
	trCurrent[4] = PT1(ctrl->tr[4], trCurrent[4], alpha_tr);
	trCurrent[5] = PT1(ctrl->tr[5], trCurrent[5], alpha_tr);
}
//-------------------------BT

void Body::setCommand() {
	/*Serial.print("joy: ");
	Serial.print(ctrl_joy1u[0]);
	Serial.print(" ");
	Serial.print(ctrl_joy1u[1]);
	Serial.print("  ");
	Serial.print(ctrl_joy2u[0]);
	Serial.print(" ");
	Serial.print(ctrl_joy2u[1]);
	Serial.print("  ax: ");
	Serial.print(ctrl_ax1u[0]);
	Serial.print(" ");
	Serial.print(ctrl_ax1u[1]);
	Serial.print("  ");
	Serial.print(ctrl_ax2u[0]);
	Serial.print(" ");
	Serial.print(ctrl_ax2u[1]);
	Serial.print(" Hight: ");
	Serial.print(" ");nMove
	Serial.print(ctrl_stepHight);
	Serial.print("  ");
	Serial.print(ctrl_roboHightu);
	Serial.print(" ");
	Serial.println(ctrl_gaitID);
	Serial.print("  tr: ");
	Serial.print(ctrl_trXYu[0]);
	Serial.print(" ");
	Serial.print(ctrl_trXYu[1]);
	Serial.print(" but: ");
	Serial.print(ctrl_buttons[0]);
	Serial.print(" ");
	Serial.println(ctrl_buttons[1]);*/

	setStepHight(ctrl->stepHight);

	setGaitUpDown(gait.selectSequence(ctrl->gaitID));

	float speedMultiplyer = 1;

	if (ctrl->gaitID == 1)
		speedMultiplyer = 1.4;
	else if (ctrl->gaitID == 2)
		speedMultiplyer = 1.2;
	else if (ctrl->gaitID == 3)
		speedMultiplyer = 1;

	if (!ctrl->buttons[0]) //walking available
	{
		setMoveParam(speedMultiplyer * 3 * saturate(ctrl->joy1u[0], -1, 1),
			ctrl->joy1u[1],
			speedMultiplyer*0.3*saturate(ctrl->ax2u[0], -1, 1),
			ctrl->nMoveMax);
		if (ctrl->buttons[1]) //aditional translation and rotation available while walking
		{
			ctrl->tr[1] = 0;
			ctrl->tr[2] = 0;
			ctrl->tr[3] = 0;
			ctrl->tr[4] = ctrl->trXYu[0];
			ctrl->tr[5] = ctrl->trXYu[1];
		}
		else
		{
			//ctrl->tr[1] = 0;
			//ctrl->tr[2] = 0;
			ctrl->tr[3] = 0;
			ctrl->tr[4] = 0.55;
			ctrl->tr[5] = 0;
		}
	}
	else
	{
		setMoveParam(0, PI / 2, 0, ctrl->nMoveMax);
		ctrl->tr[1] = 3 * ctrl->ax1u[0];
		ctrl->tr[2] = 3 * ctrl->ax1u[1];
		ctrl->tr[3] = ctrl->ax2u[0] * PI / 12;
		ctrl->tr[4] = ctrl->trXYu[0];
		ctrl->tr[5] = ctrl->trXYu[1];
	}
}

void Body::resetCommands()
{
	ctrl->nMove = 0;
}

void Body::setLinMode(bool linModeNew)
{
	ctrl->linMode = linModeNew;
}

void Body::moveOneLegGlobal(uint8_t legNo, float pointNew[3])
{
	double point[3] = {pointNew[0], pointNew[1], pointNew[2]};
	legs[legNo].setC(point);
	legs[legNo].IK();
}
