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

Hardware::Hardware(Ctrl &ctrlNew) : server(80), strip(6, LED_PIN, NEO_GRB + NEO_KHZ800)
{
	ctrl = &ctrlNew;

	servoInit();
	LEDinit();

}

void Hardware::servoPower(bool power)
{
	pinMode(SERVO_POWER_PIN, OUTPUT); //Servo power enable
	digitalWrite(SERVO_POWER_PIN, !power); //LOW for enable, HIGH for disable
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
	float servoOffset = 12 * PI / 180; //dictated by servo nature - should be removed in the future - gets calibrated
	for (int i = 0; i < 18; i++)
	{
		calibratedServos[i] = servosNew[i] + calibrationOffsets[i] + servoOffset;
		//Serial1.print(calibratedServos[i]);
		//Serial1.print(" ");
	}
	//Serial1.println();
	sc.moveAllServos(calibratedServos);
	return 0;
}

float Hardware::batteryStatus()
{
	return (float)(analogRead(BATTERY_STATUS_PIN) + 200) * 3.3 * 2 / 4096;
}

void Hardware::setCalibration(float linData[18])
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

void Hardware::wifiInit()
{
	bool ret = WiFi.softAP(ssid, passphrase, 2, 0);
	Serial.println("");
	Serial.println("WiFi AP online ...");
	server.begin();
}

void Hardware::wifiRead()
{
	//if there is no client from before try to get one.
	//this is needed to prevent searching for clients if there is one already
	if (!client)
	{
		//Serial.println("Searching for clinets...");
		client = server.available();
		if (client) Serial.println("new client");           // print a message out the serial port
	}
	//check if there is a client
	if (client) 
	{
		if (client.connected() || client.available()) 
		{
			if (client.available() >= 22) 
			{
				char small_buff[30];
				char bytes[30];
				char http_head_buff[30];
				memset(small_buff, 0, 30);
				memset(bytes, 0, 30);
				memset(http_head_buff, 0, 30);

				bytes[0] = client.read();

				if (bytes[0] == 'P') 
				{
					Serial.println("P");
					bytes[1] = client.peek();
					if (bytes[1] == 'K') 
					{
						Serial.println("K");
						client.read();
						bytes[2] = client.peek();
						if (bytes[2] == 'T') 
						{
							Serial.println("T");
							client.read();
							//client.read((uint8_t*)bytes, 19);
							//Serial.print("PKT: ");
							//for (int i = 0; i < 19; i++) { Serial.print((uint8_t)bytes[i]); Serial.print(" "); }
							//Serial.println();
						}
					}
				}

				else if (bytes[0] == 'L')
				{
					Serial.println("L");
					bytes[1] = client.peek();
					if (bytes[1] == 'I')
					{
						Serial.println("I");
						bytes[1] = client.read();
						bytes[2] = client.peek();
						if (bytes[2] == 'N')
						{
							Serial.println("N");
							bytes[2] = client.read();
							//Serial.println("LIN: ");
							//for (int i = 0; i < 18; i++) { Serial.print((uint8_t)bytes[i]); Serial.print(" "); }
							//Serial.println();
						}
					}
				}

				else if (bytes[0] == 'G') 
				{
					bytes[1] = client.peek();
					if (bytes[1] == 'E') 
					{
						client.read();
						bytes[2] = client.peek();
						if (bytes[2] == 'T') 
						{
							Serial.println("Sending confirm packet... ");
							client.read(); // 'T'
							client.read(); // ' '
							client.read((uint8_t*)http_head_buff, 16);
							Serial.print(http_head_buff);
							if (strstr(http_head_buff, "json")) 
							{
								while (client.available()) 
								{
									char c = client.read();
									Serial.print(c);
								}
								const char httpConfirmPkt[] = "HTTP/1.1 200 OK\nContent-Length: 66\n\n{  \"stemiID\": \"STEMI-657654\", \"version\": \"1.0\", \"isValid\": true}\n\n";
								Serial.println(httpConfirmPkt);
								client.write(httpConfirmPkt);
							}
                            else if (strstr(http_head_buff, "lin"))
							{
								while (client.available())
								{
									char c = client.read();
									Serial.print(c);
								}
								const char httpConfirmPkt[] = "HTTP/1.1 200 OK\n\n";
								const char linearizationPkt[] = {'L', 'I', 'N', 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0};
								Serial.print(httpConfirmPkt);
								Serial.println(linearizationPkt);
								client.write(httpConfirmPkt);
								client.write(linearizationPkt);
							}
							delay(1);
							client.stop();
						}
					}
				}
				if (ByteArrayCompare(bytes, "PKT", 3))
				{
					client.readBytes(bytes, 19);
					int i = 0;
					/*
					for(; i < 18; i++) {
					Serial.print((signed char)bytes[i]);
					Serial.print(" ");
					}
					Serial.println("    ");
					*/

					ctrl->joy1u[0] = (signed char)bytes[0] / 100.0; //r
					ctrl->joy1u[1] = -(signed char)bytes[1] * 2 * PI / 180 + PI / 2; //fi

					ctrl->ax1u[0] = ctrl->joy1u[0] * cos(ctrl->joy1u[1]);
					ctrl->ax1u[1] = ctrl->joy1u[0] * sin(ctrl->joy1u[1]);

					ctrl->joy2u[0] = (signed char)bytes[2] / 100.0; //r
					ctrl->joy2u[1] = 0; //fi

					ctrl->ax2u[0] = (signed char)bytes[2] / 100.0;
					ctrl->ax2u[1] = 0;


					ctrl->buttons[0] = bytes[3]; //static tilt
					ctrl->buttons[1] = bytes[4]; //moving tilt
					ctrl->buttons[2] = bytes[5]; //sleep

					ctrl->trXYu[0] = ((signed char)bytes[6]) / 40.0*PI / 12;
					ctrl->trXYu[1] = ((signed char)bytes[7]) / 40.0*PI / 12;

					ctrl->roboHightu = (signed char)bytes[8] / 25.0 + 2; //body hight

					ctrl->gaitID = (signed char)bytes[9] / 25;

					ctrl->stepHight = (signed char)bytes[13] / 50.0 + 1;

					if (bytes[18] > 0) ctrl->nMove = bytes[18] + 256 * bytes[17];
					else ctrl->nMove = ctrl->nMoveMax;

					ctrl->running = bytes[5];

					ctrl->linMode = 0;
					//PROVJERA
					if (
						ctrl->ax1u[0] < -1 || ctrl->ax1u[0]>1 ||
						ctrl->ax1u[1] < -1 || ctrl->ax1u[1]>1 ||
						ctrl->ax2u[0] < -1 || ctrl->ax2u[0]>1 ||
						ctrl->ax2u[1] < -1 || ctrl->ax2u[1]>1 ||

						ctrl->roboHightu < 1 || ctrl->roboHightu>6 ||
						ctrl->stepHight < 1 || ctrl->stepHight>3 ||
						ctrl->gaitID < 0 || ctrl->gaitID>4 ||

						!(ctrl->buttons[0] == 0 || ctrl->buttons[0] == 1) ||
						!(ctrl->buttons[1] == 0 || ctrl->buttons[1] == 1) ||
						!(ctrl->buttons[2] == 0 || ctrl->buttons[2] == 1) ||



						//trXY
						((signed char)bytes[6]) < -40 || ((signed char)bytes[6]) > 40 ||
						((signed char)bytes[7]) < -40 || ((signed char)bytes[7]) > 40)
					{
						i;
						/*Serial.println("==========================BLOCKADE===========================");
						ctrl->joy1u[0]=0;
						ctrl->joy1u[1]=0;
						ctrl->ax1u[0]=0;
						ctrl->ax1u[1]=0;
						ctrl->joy2u[0]=0;
						ctrl->joy2u[1]=0;
						ctrl->ax2u[0]=0;
						ctrl->ax2u[1]=0;
						ctrl->roboHightu=4;
						ctrl->gaitID=1;
						ctrl->buttons[0]=0;
						ctrl->buttons[1]=0;
						ctrl->trXYu[0]=0;
						ctrl->trXYu[1]=0;*/
					}
					Serial.print("j ");
					Serial.print(ctrl->joy1u[0]);
					Serial.print(" ");
					Serial.print(ctrl->joy1u[1]);
					Serial.print("  ");
					Serial.print(ctrl->joy2u[0]);
					Serial.print(" ");
					Serial.print(ctrl->joy2u[1]);
					Serial.print("  ax ");
					Serial.print(ctrl->ax1u[0]);
					Serial.print(" ");
					Serial.print(ctrl->ax1u[1]);
					Serial.print("  ");
					Serial.print(ctrl->ax2u[0]);
					Serial.print(" ");
					Serial.print(ctrl->ax2u[1]);
					Serial.print(" H ");
					Serial.print(ctrl->roboHightu);
					Serial.print(" SH ");
					Serial.print(ctrl->stepHight);
					Serial.print(" G ");
					Serial.print(ctrl->gaitID);
					Serial.print(" tr ");
					Serial.print(ctrl->trXYu[0]);
					Serial.print(" ");
					Serial.print(ctrl->trXYu[1]);
					Serial.print(" b ");
					Serial.print(ctrl->buttons[0]);
					Serial.print(" ");
					Serial.print(ctrl->buttons[1]);
					Serial.print(" ");
					Serial.print(ctrl->buttons[2]);
					Serial.print(" n ");
					Serial.print(ctrl->nMove);
					Serial.print(" p ");
					Serial.println(ctrl->running);
				}
				else if (ByteArrayCompare(bytes, "LIN", 3))
				{
					ctrl->linMode = 1;
					client.readBytes(bytes, 18);
					float linData[18] = { 100 - bytes[0], bytes[1],  100 - bytes[2],  100 - bytes[3],  bytes[4],  100 - bytes[5],  100 - bytes[6],  bytes[7],  100 - bytes[8],
						bytes[9], 100 - bytes[10], bytes[11], bytes[12], 100 - bytes[13], bytes[14], bytes[15], 100 - bytes[16], bytes[17] };
					for (int i; i < 18; i++)
					{
						linData[i] = (linData[i] - 50.) / 50 * 20;
						Serial.print((float)linData[i]);
						Serial.print(" ");
					}
					Serial.println();
					Serial.println("Spremio 18 brojeva lin");
					Serial.println(linData[0]);
					setCalibration(linData);
				}
			}
		}
		else 
		{// give the web browser time to receive the data
			delay(1);
			// close the connection:
			client.stop();
			Serial.println("client disonnected");
		}
	}
}

void Hardware::storageInit()
{
	preferences.begin("my-app", false);
}

void Hardware::storeCalibrationData(uint8_t linData[18])
{
	Serial.println("writing data: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print((float)linData[i]);
		Serial.print(" ");
	}
	Serial.println();
	preferences.putBytes("calibData", linData, 18);
}

void Hardware::loadCalibrationData(float linData[18])
{
	uint8_t bytes[18];
	
	Serial.println("array before reading: ");
	for (int i = 0; i < 18; i++)
	{
		Serial.print((float)linData[i]);
		Serial.print(" ");
	}
	Serial.println();

	size_t len = preferences.getBytes("calibData", bytes, 18);
	
	if (!len)
		Serial.println("Data not stored...");
	else
	{
		Serial.print("number of bytes read: ");
		Serial.println(len);
	}
	
	Serial.println("array after reading: ");
	for (int i = 0; i < 18; i++)
	{
		linData[i] = ((float)bytes[i] - 50.) / 50 * 20;
		Serial.print((float)bytes[i]);
		Serial.print(" ");
	}
	Serial.println();
}

void Hardware::LEDinit()
{
	strip.begin();
	strip.setBrightness(50);
	strip.setPixelColor(0, 255, 0, 100);
	strip.setPixelColor(1, 255, 0, 100);
	strip.setPixelColor(2, 255, 0, 100);
	strip.setPixelColor(3, 255, 0, 100);
	strip.setPixelColor(4, 255, 0, 100);
	strip.setPixelColor(5, 255, 0, 100);
	strip.show();
}

void Hardware::setAllLEDs(int bright, int r, int g, int b)
{
	strip.setBrightness(bright);
	strip.setPixelColor(0, r, g, b);
	strip.setPixelColor(1, r, g, b);
	strip.setPixelColor(2, r, g, b);
	strip.setPixelColor(3, r, g, b);
	strip.setPixelColor(4, r, g, b);
	strip.setPixelColor(5, r, g, b);
	strip.show();
}