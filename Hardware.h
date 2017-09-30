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

#include <Preferences.h>
#include "ServoController.h"
#include <NeoPixelBrightnessBus.h>
#include "Data.h"

#include "WiFi.h"

#define BUZZER_PIN 7
#define SERVO_POWER_PIN 33
#define WIFI_POWER_PIN 41
#define BATTERY_STATUS_PIN 35 //voltage is scaled to fit 3V max - see documentation
//#define ULTRASONIC_TRIGGER_PIN 34 // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ULTRASONIC_ECHO_PIN 35  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define ULTRASONIC_MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define LED_COUNT 6
#define LED_PIN 19 //Neopixel RGB LED strip pin

class Hardware
{
public:
	Hardware(Ctrl & ctrlNew);

	//Servo power
	void servoPower(bool power); //turn the servos on = 1 or off =0

	int servoInit();
	int servoWrite(float servosNew[18]);
	
	//Permanent storage
	Preferences preferences;

	//RGB LEDs
    NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip;
    //Colors
    RgbColor red = RgbColor(128, 0, 0);
    RgbColor green = RgbColor(0, 128, 0);
    RgbColor blue = RgbColor(0, 0, 128);
    RgbColor white = RgbColor(128);
    RgbColor black = RgbColor(0);

		int colorCounter = 0;
		float phaseColor = 0;

	float batteryStatus();

	void setCalibration(float linData[18]);

	void wifiInit();
	void wifiRead();

	void PKTread();
	
	void LINread();

	void GETread();

	

	void storageInit();

	void storeCalibrationData(uint8_t linData[18]);

	void loadCalibrationData();

	void LEDinit();

	void setAllLEDs(int bright, RgbColor color);

	void setAllLEDsRainbow(int bright);

	RgbColor ledPhaseColor(float phase);

	Ctrl * ctrl;

	//wifi
	WiFiServer server;
	WiFiClient client;
	char ssid[20]; // determined in wifiInit()
	const char* passphrase = "12345678";
	//const char httpConfirmPkt[100] = "HTTP/1.1 200 OK\nContent-Length: 66\n\n{  \"stemiID\": \"STEMI-657654\", \"version\": \"1.0\", \"isValid\": true}\n\n";

	//Servos
	ServoController sc;
	float calibrationOffsets[18] = {  0, 0, 0, 
																		0, 0, 0, 
																		0, 0, 0, 
																		0, 0, 0, 
																		0, 0, 0, 
																		0, 0, 0 };
	uint8_t calibrationOffsetBytes[18]{ 50, 50, 50,
																			50, 50, 50,
																			50, 50, 50,
																			50, 50, 50,
																			50, 50, 50,
																			50, 50, 50 };
};
#endif
