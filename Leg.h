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

#include "CircleIntersect.h"

#include "MatrixMath.h"

#ifndef LEG_H
#define LEG_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Leg {

public:
	Leg();
	void init(char* labelNew, double offNew, double adNew[2], double aNew[3], double trNew[6], double freqNew);
	MatrixMath Matrix;
	void IK();
	void setLinMode();
	void setC(double cNew[3]); // cNew .. [1x3]
	void setTr(double trNew[6]);
	void setQ(double qNew[3]); // qNew .. [1x3]

	void setWs(double wsScalar = 1); //set the workspace (circle)

	void setMoveParam(double moveCenterNew[2], double moveDeltaFiNew); // set the move center
	void setHomeParam(double moveDeltaNew);
	void setStepHight(double stepHightNew);
	void calcWsRange(); //calculate all phi ranges in the workspace. for current move center and current coordinates of the leg
	void calcHomeStepScale();
	void calcHomeStepRange();
	double calcFi(double center[2], double point[2]);

	void printc();
	void printq();

	void checkGaitState();
	void setGaitUpDown(double gaitUpFiNew, double gaitDownFiNew);
	void setGaitCurFi(double gaitCurFiNew);
	void incGaitCurFi(double gaitDeltaFi);
	void calcStepScale();

	float saturate(float value, float minimum, float maximum);

	void calcAll();
	void calcHomeAll();

	bool checkHomeMark();

	void move(double gaitDeltaFi, double slowingScale);
	void home(double gaitDeltaFi);

	double ground(double zAxis); //just return value, grounding it to zero
	void setCground(); //set z=0 and write to c matrix
	void earlyFootholdGround();

	char label[4];

	//sampling time and frequency
	double freq;
	double ts;


	//robot dimension and position parameters
	double ad[2];  //leg starting position [x1, x2, y], see documantation
	double off;  //angular offset of the leg, see documantation
	double a[3];  //legs link lenghts
	double q[3] = { 0,0,0 };  //legs joint angles
	double qCur[3]; //current (measured) joint angles
	double c[3];   //coordinates of the end of a leg 
	float cMat[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //coordinates of teh end of a leg (matrix)
	//coordinates of the end of the leg (matrix) (from hip perspective - coordinate system)
	float cMatInv[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	double tr[6];    //translation-rotation of the body (trans: z x y rot: z x y) 
	float T[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //Transform matrix from base to hip
	float Tinv[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //Transform matrix from hip to base

	double groundLevel; //expected ground hight

	//worksspace parameters
	double wsCenter[3];
	double wsRadius;

	//move parameters
	double moveCenter[2]; //center of the move circle in x-y
	double moveDeltaFi; //movement angle on the move circle (see documentation) [rad]
	double moveDelta; //resultatnt apsolute movement of the robot [cm]
	double stepHight;
	int stepsBeforeLanding; // number of steps before leg is expected to land
	double preFootholdHight; // hight of the leg in the final phase of the step (see documentation)
	double preFootholdScaler;

	//leg z axis dynamics
	double alpha_rise_move; // koeficijent za PT1 clan dizanja noge -> manji = brze dizanje noge
	double alpha_lower_move;
	double alpha_ground_move; // koeficijent za PT1 z=0 (guranje noge) -> za slucaj da noga nije vec u nuli da se polako spusti

	//workspace range parameters
	double moveCenterRadius;
	double fi0;  //angle to wscenter
	double fiCur;  //angle to cunrrent coordinates (c)
	double rCur; //radius to current coordinates
	double fiPlus;  //upper angle to end of workspace
	double fiMinus;  //lower angle to end of workspace

	// foothold fi
	double fiFoothold; //angle
	double rFoothold; //radius - distance
	double cFoothold[2]; //coordinates
	double pathToFoothold; //current length od the path to foothold
	//double fiToFoothold;

	bool earlyFootholdGroundedFlag; //mark that the leg had grounded early
	bool groundingState; //step phase when the leg is landing

	bool homeMark; // 1 - you foot is at home, 0 - it's not at home
	int terrainMode; //1 - using MAP corrections, 0 - not using MAP correction

	//GIAT parameters
	double gaitUpFi;  //phase of the start of raising the leg
	double gaitUpFiIdeal;
	double gaitDownFi;  //phase of lowering the leg
	double gaitDownFiIdeal;
	double gaitUpTime; //phase length gaitstate 0 (in the air) 
	double gaitDownTime;  //phase length gaitstate 1 (on the ground)

	double gaitCurFi;  //current phase of gait state
	bool gaitState;  //gait state [0-in the air, 1-on the ground]
	double gaitNext; //"time" untill the next gait change 
	double stepScalePlus; //ratio of how long can leg travel (push on the ground) -> fiPlus/gaitNext
	double stepScaleMinus; //analog to the line above
	//LEG perameters
	bool actualState; // it coud be only: 0-in the air, 1-on the ground (so far...)
	//it's the actual state of the leg. While "gaitState" is what the state should be.
	//later more states should be implemented like: rising, lowering (and ofc:in the air, on the ground)

};
#endif
