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
#include "Arduino.h"

#include "Hexapod.h"

void batteryDriver(void *sharedDataNew)
{
	BatteryDriver battery;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_BATT;
	xLastWakeTime = xTaskGetTickCount();

	battery.checkState();
	vTaskPrioritySet(NULL, 2);

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		battery.checkState();
	}
}

void robotEngine(void *sharedDataNew)
{
	RobotEngine robotEngine;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_ROBOT;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		robotEngine.checkState();
		robotEngine.modesGO();
	}
}

void walkingEngine(void *sharedDataNew)
{
	Body body;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_WALK;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		body.run();
	}
}

void servoDriver(void *sharedDataNew)
{
	ServoDriver servoDriver;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_SERVO;
	xLastWakeTime = xTaskGetTickCount();
	int k = 0;

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		if (robot.servoCtrl.store)
		{
			Serial.println("Storing calibration data ...");
			servoDriver.storeCalibrationData();
			robot.servoCtrl.store = 0;
		}
		servoDriver.servoWrite();
		servoDriver.analyseServo();
	}
}

void ledDriver(void *sharedDataNew)
{
	LedDriver ledDriver;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_LED;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		// TODO insert functions to control LEDs based on sharedData->ledCtrl variables
		if (robot.ledCtrl.mode == LED_PARAMETRIC_MODE)
		{
			ledDriver.setColorParametric();
		}
		else
		{
			ledDriver.setColorManual();
		}
		ledDriver.writeToLED();
	}
}

void btEngine(void *sharedDataNew)
{
	BluetoothLowEnergy BLE("STEMI Hexapod " + robot.name);

	delay(2000);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_BT;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		robot.BTConnectedCount = BLE.server->getConnectedCount();
		// check and publish battery level
		if ((uint8_t)BLE.batteryService->getCharacteristic(BATTERYLEVEL_CHARACTERISTIC_UUID)->getValue().c_str()[0] != robot.battery.percentage)
		{
			BLE.batteryService->getCharacteristic(BATTERYLEVEL_CHARACTERISTIC_UUID)->setValue(&robot.battery.percentage, 1);
			BLE.batteryService->getCharacteristic(BATTERYLEVEL_CHARACTERISTIC_UUID)->notify();
		}
		// check and publish robot mode
		if ((int8_t)BLE.parameterService->getCharacteristic(MODE_CHARACTERISTIC_UUID)->getValue().c_str()[0] != robot.mode)
		{
			BLE.parameterService->getCharacteristic(MODE_CHARACTERISTIC_UUID)->setValue((uint8_t *)&robot.mode, 1);
			BLE.parameterService->getCharacteristic(MODE_CHARACTERISTIC_UUID)->notify();
		}
	}
}

void touchDriver(void *sharedDataNew)
{
	Touch touch(25, 40, 5);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_TOUCH;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		touch.checkTouch();
		if (touch.isTouchDetected())
		{
			robot._setTouchPattern(touch.getTouchPattern(true));
		}
	}
}

void serialDriver(void *sharedDataNew)
{
	const TickType_t xFrequency = 10;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		checkSerial();
	}
}

void dancingEngine(void *sharedDataNew)
{
	Dance dance;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_PERIOD_DANCE;
	const TickType_t xFrequency2 = 20;

	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency2);
		if (robot.mode == ROBOT_DANCE_MODE)
		{
#ifdef FULL_DANCE
			dance.tickStart();
#endif // FULL_DANCE
			xLastWakeTime = xTaskGetTickCount();
		}
		else
		{
			robot.danceCount = 0;
			dance.setRobot(robot.danceCount);
			robot.danceCount = 0;
		}

		while (robot.mode == ROBOT_DANCE_MODE)
		{
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			delayMicroseconds(655);
			xLastWakeTime = xTaskGetTickCount();

			dance.setRobot(robot.danceCount);
			robot.danceCount++;
		}
	}
}

Hexapod::Hexapod()
{
}

void Hexapod::init(uint8_t mode)
{
	Wire.begin(23, 22);
	pinMode(PIN_A, OUTPUT);
	pinMode(PIN_B, OUTPUT);
	pinMode(PIN_C, OUTPUT);
	checkIsServerOn();
	robot.setMode(mode);
	ProductionVersion version;
	version.check();
	robot.loadName();
	delay(200);
	xTaskCreatePinnedToCore(btEngine, "btEngine", 4 * 4096, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(batteryDriver, "batteryDriver", 4 * 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE); // temporarily high priority, just for the first run
	xTaskCreatePinnedToCore(walkingEngine, "walkingEngine", 3 * 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(servoDriver, "servoDriver", 2 * 4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(ledDriver, "ledDriver", 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(robotEngine, "robotEngine", 2 * 1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(touchDriver, "touchDriver", 2 * 4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(dancingEngine, "dancingEngine", 2 * 4096, NULL, 4, NULL, ARDUINO_RUNNING_CORE);

	delay(200);
	std::string welcomeNote = "\nSTEMI Hexapod " + robot.name + " initialized :)\n\n";
	Serial.printf("%s", welcomeNote.c_str());
}