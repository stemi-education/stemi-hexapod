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


#include "ServoDriver.h"


ServoDriver::ServoDriver(SharedData *sharedDataNew)
{
	sharedData = sharedDataNew;

	storageInit();
	loadCalibrationData();
	servoPower(1);
	sharedData->servoCtrl.power = 1;
}

void ServoDriver::servoPower(bool power)
{
	pinMode(SERVO_POWER_PIN, OUTPUT); //Servo power enable
	digitalWrite(SERVO_POWER_PIN, power); //LOW for disable, HIGH for enable
}

int ServoDriver::servoWrite()
{

	if (!sharedData->servoCtrl.power)
	{
		servoPower(0);
	}
	
	//SERVO MODES
	float calibratedServos[18];
	switch (sharedData->servoCtrl.mode)
	{
	case SERVO_CALIBRATION_MODE:
		for (int i = 0; i < 6; i++)
		{
			calibratedServos[i * 3] = 0 + sharedData->servoCtrl.calibrationOffsetBytes[i * 3] / 100.0 * 0.2;
			calibratedServos[i * 3 +1] = 0 + sharedData->servoCtrl.calibrationOffsetBytes[i * 3 +1] / 100.0 * 0.2;
			calibratedServos[i * 3 +2] = -PI/2 + sharedData->servoCtrl.calibrationOffsetBytes[i * 3 +2] / 100.0 * 0.2;
		}
		sc.moveAllServos(calibratedServos);
		break;
	case SERVO_WALKING_MODE:
		//add calibration data:
		for (int i = 0; i < 18; i++)
		{
			calibratedServos[i] = sharedData->servoCtrl.servoAngles[i] + sharedData->servoCtrl.calibrationOffsetBytes[i] / 100.0 * 0.2;
			//Serial.print(calibratedServos[i]);
			//Serial.print(" ");
		}
		//Serial.println();
		sc.moveAllServos(calibratedServos);
		break;
	}
	return 0;
}

void ServoDriver::storageInit()
{
	preferences.begin("my-app", false);
}

void ServoDriver::storeCalibrationData()
{
	Serial.println("writing byte data: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print(sharedData->servoCtrl.calibrationOffsetBytes[i]);
		Serial.print(" ");
	}
	Serial.println();
	preferences.putBytes("calibData", sharedData->servoCtrl.calibrationOffsetBytes, 18);
}

void ServoDriver::loadCalibrationData()
{
	/*Serial.println("array before reading: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print((float)linData[i]);
		Serial.print(" ");
	}
	Serial.println();*/

	size_t len = preferences.getBytes("calibData", sharedData->servoCtrl.calibrationOffsetBytes, 18);
	
	if (!len)
	{
		Serial.println("Data not stored...");
		Serial.println("Loading default numbers");
		for (int i = 0; i < 18; i++)
		{
			sharedData->servoCtrl.calibrationOffsetBytes[i] = 0;
			Serial.print(sharedData->servoCtrl.calibrationOffsetBytes[i]);
			Serial.print(" ");
		}
		Serial.println();
	}
	else
	{
		Serial.print("number of bytes read: ");
		Serial.println(len);
	}
	Serial.println("float array loaded: ");

	//converting form bytes [from -100 to 100] to radians
	for (int i = 0; i < 18; i++)
	{
		
		Serial.print(((int8_t)sharedData->servoCtrl.calibrationOffsetBytes[i]) / 100.0 * 0.2);
		Serial.print(" ");
	}
	Serial.println();
	Serial.println("byte array loaded: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print(sharedData->servoCtrl.calibrationOffsetBytes[i]);
		Serial.print(" ");
	}
	Serial.println();
}