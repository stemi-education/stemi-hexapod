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


#ifndef BODY_H
#define BODY_H

#include "Gait.h"
#include "Leg.h"
#include "Data.h"

class Body {
public:
	Body(Ctrl &ctrlNew, Parameters &parametersNew); // constructor
	void packQArray();
	void IK(); // sets all q's from wanted coordinates of each leg ---> q[3] = joint angles
	void setLinMode(); // set the robot in servo linearisation mode
	void setTr(double trNew[6]); //sets the T and Tinv matrices acording to tr vector    
	void trPT1(); //PT1 filter for translation and rotation
	void printq(); // prints all joint angles
	void printc(); // prints all final coordinates of legs

	float saturate(float value, float minimum, float maximum);

	//-----------GAIT and WS-----------------

	void setWs(double wsScalar = 1);
	void calcAll();
	void calcHomeAll();
	void incGaitFi(double gaitStep);
	void setGaitUpDown(double gaitArray[12]);
	void setGaitCurFi(double gaitCurFiNew);
	void setMoveParam(double speedNew, double fiNew, double deltaFiNew, int nMoveNew);
	void setRotateParam(double  moveDeltaFiNew);
	void setHomeParam(double moveDeltaNew);
	void setStepHight(double stepHightNew);
	void scaleStepFi();
	void scaleHomeStep();

	bool checkHomeMark();

	void move();
	void home(float moveDeltaNew);
	void run();

	void setCground();
	void setCommand();
	void setLinMode(bool linModeNew);

	void moveOneLegGlobal(uint8_t legNo, float pointNew[3]);

	void resetCommands();

	Leg legs[6] = { Leg() };

	Gait gait;

	Parameters *parameters;
	Ctrl *ctrl;

	//sampling time
	double ts; //TODO move ts to ctrl in data.h

	//limb angles:
	float qAll[18];
//private:

	// parameters
	double tr[6]; //temporary saves the commands for further procesing. Exists because of timing problems.
	double trCurrent[6] = { 0,0,0,0,0,0 }; //real tr after applying PT1 filter
	double alpha_tr; //parametar for PT1 filter
	double speed;
	double maxAllowedSpeed;

	uint8_t walkingLegsMap[6] = { 1,2,4,5, 0, 3 }; //what legs are used for walking
	uint8_t nWalkingLegs = 4; //number of used legs for walking on the robot
	uint8_t nLegs = 6; //all installed legs on robot - 6 for hexapod 8 for octopod

	double moveDelta;  //delta body movement 
	double moveDeltaFi; //delta fi for body movement
	double gaitDeltaFi; //delta fi for gait sequence

	double gaitUpTime; //phase length gaitstate 0 (in the air) 
	double gaitDownTime;  //phase length gaitstate 1 (on the ground)

	double slowingScale; // factor that slows movement of the robot

};
#endif
