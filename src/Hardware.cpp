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


#include "Hardware.h"


float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Hardware::Hardware(SharedData *sharedDataNew) : strip(LED_COUNT, LED_PIN), touch(50, 40, 5)
{
	sharedData = sharedDataNew;

	storageInit();
	servoInit();
	LEDinit();
	loadCalibrationData();
	Serial.println("check end hardware...");
}

void Hardware::servoPower(bool power)
{
	pinMode(SERVO_POWER_PIN, OUTPUT); //Servo power enable
	digitalWrite(SERVO_POWER_PIN, power); //LOW for disable, HIGH for enable
}


int Hardware::servoInit()
{
	Serial.println("INICIJALIZIRAM!!!!!!");
	return 0;
}

int Hardware::servoWrite(float servosNew[18])
{
	float calibratedServos[18];
	//add calibration data:
	float servoOffset = 0;// 12 * PI / 180; //dictated by servo nature - should be removed in the future - gets calibrated
	for (int i = 0; i < 18; i++)
	{
		calibratedServos[i] = servosNew[i] + calibrationOffsets[i] + servoOffset;
		//Serial.print(calibratedServos[i]);
		//Serial.print(" ");
	}
	//Serial.println();
	sc.moveAllServos(calibratedServos);
	return 0;
}

float Hardware::batteryStatus()
{
	float senVal = (float)(analogRead(BATTERY_STATUS_PIN));
	return(-0.000000000023926 * pow(senVal, 3) + 0.000000094746 * pow(senVal, 2) + 0.00074539 * senVal + 0.14925) * 2.0;
}

void Hardware::setCalibration(int8_t linData[18])
{
	for (int i = 0; i < 18; i++)
	{
		calibrationOffsets[i] = -linData[i] * (PI / 180);
	}
}

boolean ByteArrayCompare(char a[], const char b[], int array_size)
{
	for (int i = 0; i < array_size; ++i)
		if (a[i] != b[i])
			return(false);
	return(true);
}

void Hardware::storageInit()
{
	preferences.begin("my-app", false);
}

void Hardware::storeCalibrationData(int8_t linData[18])
{
	Serial.println("writing byte data: ");
	for (int i = 0; i < 18; i++)
	{
		calibrationOffsetBytes[i] = linData[i];
		Serial.print(linData[i]);
		Serial.print(" ");
	}
	Serial.println();
	preferences.putBytes("calibData", linData, 18);
}

void Hardware::loadCalibrationData()
{
	/*Serial.println("array before reading: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print((float)linData[i]);
		Serial.print(" ");
	}
	Serial.println();*/

	size_t len = preferences.getBytes("calibData", calibrationOffsetBytes, 18);
	
	if (!len)
	{
		Serial.println("Data not stored...");
		Serial.println("Loading default numbers");
		for (int i = 0; i < 18; i++)
		{
			calibrationOffsetBytes[i] = 0;
			Serial.print(calibrationOffsetBytes[i]);
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
		calibrationOffsets[i] = ((int8_t)calibrationOffsetBytes[i]) / 100.0 * 0.2;
		Serial.print(calibrationOffsets[i]);
		Serial.print(" ");
	}
	Serial.println();
	Serial.println("byte array loaded: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print(calibrationOffsetBytes[i]);
		Serial.print(" ");
	}
	Serial.println();
}

void Hardware::LEDinit()
{
	strip.Begin();
	strip.SetBrightness(50);
	strip.SetPixelColor(0, RgbColor(255, 0, 0));
	strip.SetPixelColor(1, RgbColor(100, 100, 0));
	strip.SetPixelColor(2, RgbColor(0, 255, 0));
	strip.SetPixelColor(3, RgbColor(0, 100, 100));
	strip.SetPixelColor(4, RgbColor(0, 0, 255));
	strip.SetPixelColor(5, RgbColor(100, 0, 100));
	delay(1);
	strip.Show();
}

void Hardware::setAllLEDs(int bright, RgbColor color)
{
	strip.SetBrightness(bright);
	strip.SetPixelColor(0, color);
	strip.SetPixelColor(1, color);
	strip.SetPixelColor(2, color);
	strip.SetPixelColor(3, color);
	strip.SetPixelColor(4, color);
	strip.SetPixelColor(5, color);
	delay(1);
	strip.Show();
}

void Hardware::setAllLEDsRainbow(int bright)
{
	colorCounter++;
	if (colorCounter == 100)
		colorCounter = 0;
	phaseColor = colorCounter / 100.0 * 2 * PI;
	strip.SetBrightness(bright);
	strip.SetPixelColor(0, ledPhaseColor(0));
	strip.SetPixelColor(1, ledPhaseColor(2 * PI / 6));
	strip.SetPixelColor(2, ledPhaseColor(2 * 2 * PI / 6));
	strip.SetPixelColor(3, ledPhaseColor(2 * 3 * PI / 6));
	strip.SetPixelColor(4, ledPhaseColor(2 * 4 * PI / 6));
	strip.SetPixelColor(5, ledPhaseColor(2 * 5 * PI / 6));
	delay(1);
	strip.Show();
}

RgbColor Hardware::ledPhaseColor(float phase)
{
	return RgbColor((int)((sin(phaseColor + phase) / 2.0 + 0.5) * 255), (int)((sin(phaseColor + phase + 2 / 3 * PI) / 2.0 + 0.5) * 255), (int)((sin(phaseColor + phase + 4 / 3 * PI) / 2.0 + 0.5) * 255));
}