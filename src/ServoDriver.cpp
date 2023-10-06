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

ServoDriver::ServoDriver()
{
	delay(200);
	storageInit();
	loadCalibrationData();
}

void ServoDriver::servoPower(bool power)
{
	pinMode(SERVO_POWER_PIN, OUTPUT);	  // Servo power enable
	digitalWrite(SERVO_POWER_PIN, power); // LOW for disable, HIGH for enable
}

int ServoDriver::servoWrite()
{
	servoPower(robot.servoCtrl.power);

	// SERVO MODES
	float calibratedServos[18];
	switch (robot.servoCtrl.mode)
	{
	case SERVO_CALIBRATION_MODE:

		for (int i = 0; i < 6; i++)
		{
			calibratedServos[i * 3] = 0 + robot.servoCtrl.calibrationOffsetBytes[i * 3] / 100.0 * 0.2;
			calibratedServos[i * 3 + 1] = 0 + robot.servoCtrl.calibrationOffsetBytes[i * 3 + 1] / 100.0 * 0.2;
			calibratedServos[i * 3 + 2] = -PI / 2 + robot.servoCtrl.calibrationOffsetBytes[i * 3 + 2] / 100.0 * 0.2;
		}
		sc.moveAllServos(calibratedServos);

		if (robot.servoCtrl.nudge > -1) // if nudge is required for some servo layer
		{
			int delayTime = 10;
			// Serial.println(calibrationServoLayerSelected);
			float nudgeAmmount = 0.1; // radians
			for (int j = 0; j < 20; j++)
			{
				for (int i = 0; i < 18; i++)
				{
					if (i % 3 == robot.servoCtrl.nudge)
						calibratedServos[i] += nudgeAmmount / delayTime;
				}
				sc.moveAllServos(calibratedServos);
				delay(delayTime);
			}
			for (int j = 0; j < 40; j++)
			{
				for (int i = 0; i < 18; i++)
				{
					if (i % 3 == robot.servoCtrl.nudge)
						calibratedServos[i] -= nudgeAmmount / delayTime;
				}
				sc.moveAllServos(calibratedServos);
				delay(delayTime);
			}
			for (int j = 0; j < 20; j++)
			{
				for (int i = 0; i < 18; i++)
				{
					if (i % 3 == robot.servoCtrl.nudge)
						calibratedServos[i] += nudgeAmmount / delayTime;
				}
				sc.moveAllServos(calibratedServos);
				delay(delayTime);
			}
			robot.servoCtrl.nudge = -1;
		}
		break;
	case SERVO_WALKING_MODE:
		// add calibration data:
		for (int i = 0; i < 18; i++)
		{
			calibratedServos[i] = robot.servoCtrl.servoAngles[i] + robot.servoCtrl.calibrationOffsetBytes[i] / 100.0 * 0.2;
			// Serial.print(calibratedServos[i]);
			// Serial.print(" ");
		}
		// Serial.println();
		sc.moveAllServos(calibratedServos, robot.servoCtrl.extraServoAngle);
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
	preferences.putBytes("calibData", robot.servoCtrl.calibrationOffsetBytes, 18);
}

void ServoDriver::loadCalibrationData()
{
	size_t len = preferences.getBytes("calibData", robot.servoCtrl.calibrationOffsetBytes, 18);

	if (!len)
	{
		Serial.println("Servo calibration data not stored");
		Serial.println("Loading default numbers: ");
		for (int i = 0; i < 18; i++)
		{
			robot.servoCtrl.calibrationOffsetBytes[i] = 0;
			Serial.print(robot.servoCtrl.calibrationOffsetBytes[i]);
			Serial.print(" ");
		}
		Serial.println();
	}
	else
	{
		Serial.println("Servo calibration data loaded:");
		for (int i = 0; i < 18; i++)
		{
			Serial.print(robot.servoCtrl.calibrationOffsetBytes[i]);
			Serial.print(" ");
		}
		Serial.println();
	}
}

void setMuxChannel(int channel)
{
	digitalWrite(PIN_A, bitRead(channel, 0));
	digitalWrite(PIN_B, bitRead(channel, 1));
	digitalWrite(PIN_C, bitRead(channel, 2));
}

void ServoDriver::analyseServo()
{
	// TODO
	return;
	for (int i = 0; i < 7; i++)
	{
		setMuxChannel(i);
		Serial.print((float)((float)analogRead(PIN_AD) / 4095.0) / 3.3 * 0.1 * 50 * 1000);
		Serial.print(" ");
	}
	setMuxChannel(7);
	float voltage = (float)((float)analogRead(35) / 4095.0) * 3.3 * 2;
	Serial.println(voltage);
}
