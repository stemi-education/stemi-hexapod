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


#ifndef DANCE_H
#define DANCE_H

#define ST_0 0,	0

#define S_L -50,0
#define S_R 50, 0
#define S_F 0,	50
#define S_B 0,	-50

#define T_L 0,	-50
#define T_R 0,	50
#define T_F -50,0
#define T_B 50,	0

#define P0 0
#define PL 1
#define PR 2
#define PF 3
#define PB 4

#define M0 0
#define MF 1
#define MB 2
#define ML 3
#define Mr 4
#define RL 5
#define RR 6

#define HM 0
#define HL 1
#define HH 2
#define HC 3
#define H0 4

#define C0 0
#define CR 1
#define CG 2
#define CB 3
#define CY 4
#define CC 5
#define CP 6

#define M0 0
#define M1 1
#define M2 2
#define M3 3
#define M4 4
#define M5 5
#define M6 6
#define M7 7
#define M8 8
#define M9 9


#include "SharedData.h"

class Dance
{
public:
	Dance();
	void setRobot(uint j);

	void tickStart();



	uint8_t movesArray[460] =
	{
		//orchestral
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//intro
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,

		//first
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//second
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//transition
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,

		//solo
		MF,	MF,  MF,	MF,	0,	0,	0,	0,		RR,	RR,  RR,	RR,	0,	0,	0,	0,		RL,	RL,  RL,	RL,	0,	0,	0,	0,		MB,	MB,  MB,	MB,	0,	0,	0,	0,
		RR,	RR,  RR,	RR,	0,	0,	0,	0,		RL,	RL,  RL,	RL,	0,	0,	0,	0,

		//orchestral
		RR,	RR, RR,	RR,	RR,	RR, 0,	0,			MF,	0,  MB,	0,	MF,	0,	MB,	0,			RL,	RL, RL,	RL,	RL,	RL,	0,	0,			MF,	0,  MB,	0,	MF,	0,	MB,	0,
		ML,	ML,  ML,	0, 0,	0,	0,	0,			Mr,	Mr,  Mr,	Mr,	0,	0,	0,	0,		Mr,	Mr,  Mr,	Mr,	0,	0,	0,	0,		ML,	ML,  ML,	ML, 0,	0,	0,	0,

		//third
		0,	0,  0,	0,	0,	0,	0,	0,			MF,	0,  MB,	0,	MF,	0,	MB,	0,			0,	0,  0,	0,	0,	0,	0,	0,			ML,	0,  Mr,	0,	ML,	0,	Mr,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			MF,	0,  MB,	0,	MF,	0,	MB,	0,			0,	0,  0,	0,	0,	0,	0,	0,			ML,	0,  Mr,	0,	0,	0,	0,	0,

		//fourth
		0,	0,  0,	0,	0,	0,	0,	0,			MF,	0,  MB,	0,	MF,	0,	MB,	0,			0,	0,  0,	0,	0,	0,	0,	0,			ML,	0,  Mr,	0,	ML,	0,	Mr,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0
	};
	uint8_t heightArray[460] =
	{
		//orchestral 0
		H0,	H0,  H0,	H0,	H0,	H0,	H0,	H0,		H0,	H0,  H0,	H0,	H0,	H0,	H0,	H0,		H0,	H0,  H0,	H0,	H0,	H0,	H0,	H0,		H0,	H0,  H0,	H0,	H0,	H0,	H0,	H0,

		//intro 32
		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,
		HM,	HC,  HM,	HC,	HL,	HL,	HL,	HL,
	
		//first  72
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HL,  HM,	HL,	HM,	0,	0,	0,	0,

		//second 128
		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,
		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,		HM,	HC,  HM,	HC,	HM,	HC,	HM,	HC,
	
		//transition 176
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,
			
		//solo 228
		0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	HC,	HC,	HC,	HC,
		0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	0,	0,  0,	0,
	
		//orchestral 276
		0,	0,  0,	0,	0,	0,	0,	0,			0,	HC,  0,	HC,	0,	HC,	0,	HC,			0,	0,  0,	0,	0,	0,	0,	0,			0,	HC,  0,	HC,	0,	HC,	0,	HC,
		0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	0,	0,  0,	0,

		//third 340
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,	0,	0,	0,	0,			HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,	0,	0,	0,	0,
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,	0,	0,	0,	0,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,			0,	0,  0,	0,	0,	0,	0,	0,

		//fourth 404
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,	0,	0,	0,	0,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,
		HC,  HC,	HC,	HC,	HC,	HC,	HC,	HC,		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	H0
	};
	uint8_t poseArray[460] =
	{
		//orchestral
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//intro
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,

		//first
		0,	0,  0,	0,	0,	0,	0,	0,			PL,	PL,	PL,	PL,	PL,	PL,	PL,	PL,			PR,	PR,	PR,	PR,	PR,	PR,	PR,	PR,			PL,	PL,	PL,	PL,	PR,	PR,	PR,	PR,
		PL,	PL, PR,	PR,	PL,	PL,	PR,	PR,			PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//second
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			PL,	0, PR,	0,	0,	0,	0,	0,

		//transition
		0,	0,  0,	0,	0,	0,	0,	0,			PL,	PL,	PL,	PL,	PL,	PL,	PL,	PL,			PR,	PR,	PR,	PR,	PR,	PR,	PR,	PR,			PL,	PL,	PL,	PL,	PR,	PR,	PR,	PR,
		PL,	PL, PR,	PR,	PL,	PL,	PR,	PR,			PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,

		//solo
		PF,	PF,  PF,	PF,	0,	PR,	0,PL,		0,	0,  0,	0,	0,	PR,	0,	PL,			0,	0,  0,	0,	0,	PR,	0,	PL,			0,	0,  0,	0,	0,	PR,	0,	PL,
		0,	0,  0,	0,	0,	PR,	0,	PL,			0,	0,  0,	0,	0,	0,	0,	0,

		//orchestral
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		PL,	PL,  PL,	PL,	0,	PR,	0,	PL,		PR,	PR,  PR,	PR,	0,	PL,	0,	PR,		PR,	PR,  PR,	PR,	0,	PL,	0,	PR,		PL,	PL,  PL,	PL,	0,	0,	0,	0,

		//third
		PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,			PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,			PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//fourth
		PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,			PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		PL,	0, PR,	0,	PL,	0,	PR,	0,			0,	0,  0,	0,	0,	0,	0,	0,			PF,	PF,  PF,	PF,	PF,	PF,	PF,	0
	};

	uint8_t ledColorArray[460] =
	{
		//orchestral
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//intro 32
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,

		//first 
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	0,	0,	0,	0,

		//second
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	CY,	CR, CB,	CP,

		//transition
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	CY,	CR, CB,	CP,		0,	0,  0,	0,

		//solo
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	0,	0,	0,	0,

		//orchestral
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	CY,	CR, CB,	CP,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	0,	0,	0,	0,

		//third
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		CY,	CR,  CB,	CP,	CY,	CR, CB,	CP,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	0,	0,	0,	0,

		//fourth
		CB,	CB,  CB,	CB,	CB,	CB,	CB,	CB,		CY,	CY,  CY,	CY,	CG,	CG,	CG,	CG,		CR,	CR, CR,	CR,CP,	CP,	CP,	CP,			CY,	CR,  CY,	CR,	CG,	CC,	CG,	CC,
		CY,	CR,  CB,	CP,	CC,	CG,	CR,	CB,		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0
	};

	uint8_t ledAnimationArray[460] =
	{
		//orchestral
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//intro
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,

		//first
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,
		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0,

		//second
		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,
		M2,	M2,  M2,	M2,	M2,	M2,	M2,	M2,		M2,	M2,  M2,	M2,	0,	0,	0,	0,

		//transition
		M0,	M5,  M0,	M6,	M0,	M5,	M0,	M7,		M0,	M8,  M0,	M5,	M0,	M8,	M0,	M5,		M0,	M5,  M0,	M6,	M0,	M5,	M0,	M7,		M0,	M8,  M0,	M5,	M0,	M8,	M0,	M5,
		M5,	M7,  M5,	M7,	M5,	M7,	M5,	M7,		M5,	M7,  M5,	M7,	M5,	M7,	M5,	M7,		0,	0,  0,	0,

		//solo
		0,	0,  0,	0,	0,	M2,	0,	M4,			0,	0,  0,	0,	0,	M2,	0,	M4,			0,	0,  0,	0,	0,	M2,	0,	M4,			0,	0,  0,	0,	0,	M2,	0,	M4,
		M6,	M7,  M6,M7,M6,	M7,  M6,M7,			M6,	M7,  M6,	M7,	0,	0,	0,	0,

		//orchestral
		M0,	M5,  M0,	M6,	M0,	M5,	M0,	M7,		M0,	M8,  M0,	M5,	M0,	M8,	M0,	M5,		M0,	M5,  M0,	M6,	M0,	M5,	M0,	M7,		M0,	M8,  M0,	M5,	M0,	M8,	M0,	M5,
		M5,	M7,  M5,	M7,	M5,	M7,	M5,	M7,		M5,	M7,  M5,	M7,	M5,	M7,	M5,	M7,		M0,	M8,  M0,	M5,	M0,	M8,	M0,	M5,		M0,	M5,  M0,	M6,	0,	0,	0,	0,

		//third
		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,
		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,		M0,	M2,  M0,	0,	0,	0,	0,

		//fourth
		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,		M0,	M2,  M0,	M1,	M0,	M2,	M0,	M1,
		M0,	M1,  M0,	M1,	M0,	M1,	M0,	M1,		0,	0,  0,	0,	0,	0,	0,	0,			0,	0,  0,	0,	0,	0,	0,	0
	};

	MovementData movements[7] = 
	{
		{ 0,		0,		0 }, //M0 0
		{ 30,		0,			0 }, //MF 1
		{ 30,		180,		0 }, //MB 2
		{ 30,		90,				0 }, //ML 3
		{ 30,		-90,		0 },	 //Mr 4
		{ 0,		90,				100 }, //RL 5
		{ 0,		90,				-100 } //RR 6
	};
	uint8_t heights[5] = { 50, 40, 65, 100, 20};
	PoseData poses[17] = 
	{ 
		//reset
		{	0,	0,	50,		0,	0,	0 }, // 0

		//stretch
		{ -50, 0,	50,		0,	0,	30 }, // PL 1
		{ 50, 0,	50,		0,	0,	-30 }, // PR 2
		{ 0, 0,	0,		-30,	0,	0 }, // PF 3
		{ 0, 0,	0,		60,	0,	0 }, // PB 4
	};
	LedData ledColors[7] = 
	{
		{ { 0,0,0 },				{ 0,0,0 },			0, 20}, //C0
		{ { 255,0,0 },			{ 0,0,0 },			0, 20}, //CR
		{ { 0,255,0 },			{ 0,0,0 },			0, 20}, //CG
		{ { 0,0,255 },			{ 0,0,0 },			0, 20}, //CB
		{ { 255,242,0},			{ 0,0,0 },			0, 20}, //CY
		{ { 0,255,255 },		{ 0,0,0 },			0, 20}, //CC
		{ { 0,255,255 },		{ 0,0,0 },			0, 20}, //CP
	};
	LedData ledAnimations[9] =
	{
		{ { 0,0,0 },{ 0,0,0 },			0, 0, 0, 0 }, //M0
		{ { 0,0,0 },{ 0,0,0 },			0, 0, 18, 0 }, //M1
		{ { 0,0,0 },{ 0,0,0 },			0, 0, 2 * 18, 0 }, //M2
	{ { 0,0,0 },{ 0,0,0 },			0, 0, -18, 0 }, //M3
	{ { 0,0,0 },{ 0,0,0 },			0, 0, -2 * 18, 0 }, //M4
	{ { 0,0,0 },{ 0,0,0 },			0, 0, 0, 18 }, //M5
	{ { 0,0,0 },{ 0,0,0 },			0, 0, 0, 2 * 18 }, //M6
	{ { 0,0,0 },{ 0,0,0 },			0, 0, 0, 50 }, //M7
	{ { 0,0,0 },{ 0,0,0 },			0, 0, 0, 100} //M8
	};
};
#endif

