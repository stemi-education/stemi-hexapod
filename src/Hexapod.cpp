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

void robotEngine(void *sharedDataNew)
{
	SharedData *sharedData = (SharedData*)sharedDataNew;
  RobotEngine robotEngine(sharedData);

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
	SharedData *sharedData = (SharedData*)sharedDataNew;
	Body body(sharedData);

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
	SharedData *sharedData = (SharedData*)sharedDataNew;
	ServoDriver servoDriver(sharedData);
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		if (sharedData->servoCtrl.store)
		{
			Serial.println("Storing calibration data ...");
			//servoDriver.storeCalibrationData();
			sharedData->servoCtrl.store = 0;
		}
		servoDriver.servoWrite();
	}
}

void batteryDriver(void *sharedDataNew)
{
	SharedData *sharedData = (SharedData*)sharedDataNew;
	BatteryDriver battery(sharedData);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		battery.checkState();
	}
}

void ledDriver(void *sharedDataNew)
{
	SharedData *sharedData = (SharedData*)sharedDataNew;
	LedDriver ledDriver(sharedData);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 30;
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//TODO insert functions to control LEDs based on sharedData->ledCtrl variables
		ledDriver.setColorParametric();
	}
}

void BtEngine(void *sharedDataNew)
{
	SharedData *sharedData = (SharedData*)sharedDataNew;
	BluetoothLowEnergy BLE(std::string("STEMIHexapod"), sharedData);
	delay(2000);

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = portMAX_DELAY; //wait forever
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//Serial.println("BT");
	}
}

Hexapod::Hexapod()
{
	Serial.begin(115200);
	Serial.println("Hexapod init");

	xTaskCreatePinnedToCore(walkingEngine, "walkingEngine", 3*4096, (void*)&sharedData, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(servoDriver, "servoDriver", 2*4096, (void*)&sharedData, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(batteryDriver, "batteryDriver", 1024, (void*)&sharedData, 2, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(ledDriver, "ledDriver", 1024, (void*)&sharedData, 1, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(robotEngine, "robotEngine", 1024, (void*)&sharedData, 3, NULL, ARDUINO_RUNNING_CORE);
	xTaskCreatePinnedToCore(BtEngine, "BtEngine", 2*4096, (void*)&sharedData, 3, NULL, ARDUINO_RUNNING_CORE);


}