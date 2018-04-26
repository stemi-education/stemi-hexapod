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
	const TickType_t xFrequency = 200;
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
	const TickType_t xFrequency = 20;
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
	const TickType_t xFrequency = 20;
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
	const TickType_t xFrequency = 20;
	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		if (robot.servoCtrl.store)
		{
			Serial.println("Storing calibration data ...");
			//servoDriver.storeCalibrationData();
			robot.servoCtrl.store = 0;
		}
		servoDriver.servoWrite();
	}
}

void ledDriver(void *sharedDataNew)
{
	LedDriver ledDriver;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//TODO insert functions to control LEDs based on sharedData->ledCtrl variables
		if (robot.ledCtrl.mode == LED_PARAMETRIC_MODE)
			ledDriver.setColorParametric();
		else
			ledDriver.setColorManual();
		ledDriver.writeToLED();
	}
}

void btEngine(void *sharedDataNew)
{
	BluetoothLowEnergy BLE(std::string("STEMIHexapod"));
	delay(2000);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		robot.BTConnectedCount = BLE.server->getConnectedCount();
	}
}

void touchDriver(void *sharedDataNew)
{
	Touch touch(50, 40, 5);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		touch.checkTouch();
		if (touch.isTouchDetected())
		{
			robot.setTouchPattern(touch.getTouchPattern(true));
		}
	}
}

Hexapod::Hexapod()
{
	Serial.begin(115200);
	Serial.println("Hexapod init");

	xTaskCreatePinnedToCore(batteryDriver, "batteryDriver", 1024, NULL, 5, NULL, ARDUINO_RUNNING_CORE); //temporarily high priority, just for the first run
	xTaskCreatePinnedToCore(walkingEngine, "walkingEngine", 3*4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(servoDriver, "servoDriver", 2*4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(ledDriver, "ledDriver", 1024, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(robotEngine, "robotEngine", 1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(btEngine, "BtEngine", 2 * 4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(touchDriver, "touchDriver", 2*4096, NULL, 3, NULL, ARDUINO_RUNNING_CORE);

}