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


#include "Gait.h"

GaitSequence::GaitSequence() {};

void GaitSequence::set(char* labelNew, double sequenceNew[12], double allDownTime, double offset[6], double startPositionNew) {
	sequence[0] = sequenceNew[0] - offset[0];
	sequence[1] = sequenceNew[1] - offset[0] - allDownTime;

	sequence[2] = sequenceNew[2] - offset[1];
	sequence[3] = sequenceNew[3] - offset[1] - allDownTime;

	sequence[4] = sequenceNew[4] - offset[2];
	sequence[5] = sequenceNew[5] - offset[2] - allDownTime;

	sequence[6] = sequenceNew[6] - offset[3];
	sequence[7] = sequenceNew[7] - offset[3] - allDownTime;

	sequence[8] = sequenceNew[8] - offset[4];
	sequence[9] = sequenceNew[9] - offset[4] - allDownTime;

	sequence[10] = sequenceNew[10] - offset[5];
	sequence[11] = sequenceNew[11] - offset[5] - allDownTime;

	startPosition = startPositionNew;

	//label = labelNew;
	strcpy(label, labelNew);
};


Gait::Gait() {
	double tripodBase[12] = { 3, 0,
														0, 3,
														3, 0,
														0, 3,
														3, 0,
														0, 3 };

	double rippleBase[12] = { 0, 2,
														4, 0,
														2, 4,
														3, 5,
														1, 3,
														5, 1 };

	double waveBase[12] = { 0, 1,
													1, 2,
													2, 3,
													3, 4,
													4, 5,
													5, 0 };

	double offsetZero[6] = { 0, 0, 0, 0, 0, 0 };
	double offsetMod2[6] = { 0.8, 0.8, 0.4, 0.4, 0, 0 };

	gaitSequence[0].set("tripodBase ", tripodBase, 0, offsetZero, 0);
	gaitSequence[1].set("tripodMod1 ", tripodBase, 0.5, offsetZero, 0);
	gaitSequence[2].set("tripodMod2 ", tripodBase, 0.8, offsetMod2, 0);
	gaitSequence[3].set("rippleBase ", rippleBase, 0, offsetZero, 0);
	gaitSequence[4].set("waveBase ", waveBase, 0, offsetZero, 0);
}

double * Gait::selectSequence(int index) {
	return gaitSequence[index].sequence;
}
double Gait::selectStart(int index) {
	return gaitSequence[index].startPosition;
}

