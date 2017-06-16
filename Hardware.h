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


#ifndef HARDWARE_H
#define HARDWARE_H

#include "DueTimer.h"
#include "NewPing.h"
#include <Servo.h>
#include "Data.h"

#define BUZZER_PIN 7
#define SERVO_POWER_PIN 8
#define WIFI_POWER_PIN 41
#define BATTERY_STATUS_PIN A0 //voltage is scaled to fit 3V max - see documentation
#define LED_ARRAY_START_PIN 2
#define LED_ARRAY_END_PIN 6
#define ULTRASONIC_TRIGGER_PIN 34 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ULTRASONIC_ECHO_PIN 35  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define ULTRASONIC_MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

class Hardware
{
public:
	Hardware(Ctrl & ctrlNew);

	//sound
	void soundHandler(); //timer handler used for sound
	static void soundHandlerWrapper(); //just to satisfy c++ static function demands
	void playSound(float frequency, int duration); //play a sound on a buzzer

	void soundInit();
	void ultrasonicInit();

	//LED
	void ledInit();

	void setLED(int nLED, float power);

	//Servo power
	void servoPower(bool power); //turn the servos on = 1 or off =0

	//wifi
	void wifiReset(); //after startup wifi needs to be reset in order to work properly (its power supply needs to be smooth)

	float batteryStatus();

	int getDistance();

	int servoInit();
	int servoWrite(float servosNew[18]);
	void setCalibration(float linData[18]);

	void wifiRead();

	Ctrl * ctrl;

	NewPing ultrasonic;

	//variables
	bool soundOn;
	long soundCounter;
	long soundDuration;

	int lastDistanceMeasuremeTime;//ultrasonic time measurement to limit the measurement frequency

	int batteryVoltage;

	//Servos
	Servo servos[18];
	float calibrationOffsets[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};
#endif
