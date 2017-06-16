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

Hardware* global_hardware;

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Hardware::Hardware(Ctrl &ctrlNew) : ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_MAX_DISTANCE)
{
	ctrl = &ctrlNew;

	servoInit();
	global_hardware = this; //just a pointer to this class. Used in static functions
	soundInit(); //initialise sound
	ultrasonicInit(); //initialise ultrasonic dispance sensor
}

void Hardware::soundHandler()
{
	soundOn = !soundOn;

	digitalWrite(BUZZER_PIN, soundOn); // sound on off
	soundCounter++;
	if (soundCounter > soundDuration) //turn off the timer and the sound after soundDuration
	{
		Timer1.stop();
		soundOn = 0;
		digitalWrite(BUZZER_PIN, soundOn); // sound on off
		soundCounter = 0;
	}
}

void Hardware::soundHandlerWrapper()
{
	global_hardware->soundHandler();
}

void Hardware::playSound(float frequency, int duration)
{
	soundOn = 0;
	soundCounter = 0;
	soundDuration = duration / 1000.* frequency * 2; //set the duration in ms and convert it to the number of ticks of the handler on the frequency given
	Timer1.start(int(1 / frequency * 1000000) * 2); // Calls every x us, 1/x KHz
	//*2 is because there is 2 events (on and off) in one period of the sound signal
}
void Hardware::soundInit()
{
	pinMode(BUZZER_PIN, OUTPUT); //buzzer pin init
	//initialise variables for sound handling
	soundOn = 0; //this variable toggles on and off to make a sound
	soundCounter = 0; //used for duration of the sound
	soundDuration = 3000;
	Timer1.attachInterrupt(&Hardware::soundHandlerWrapper);
}

void Hardware::ultrasonicInit()
{
	lastDistanceMeasuremeTime = 0;
}
void Hardware::ledInit()
{
	for (int i = LED_ARRAY_START_PIN; i <= LED_ARRAY_END_PIN; i++) // turn on LEDs on pins [2-6]
	{
		pinMode(i, OUTPUT);
		analogWrite(i, 255);
	}
}

void Hardware::setLED(int nLED, float power)
{
	analogWrite(nLED - 1, power * 255);
}

void Hardware::servoPower(bool power)
{
	pinMode(SERVO_POWER_PIN, OUTPUT); //Servo power enable
	digitalWrite(SERVO_POWER_PIN, !power); //LOW for enable, HIGH for disable
}

void Hardware::wifiReset()
{
	pinMode(WIFI_POWER_PIN, OUTPUT);
	digitalWrite(WIFI_POWER_PIN, LOW);//turn off WIFI
	delay(500); //just a small delay so wifi chip can read the signal
	digitalWrite(WIFI_POWER_PIN, HIGH);//turn on WIFI
}

float Hardware::batteryStatus()
{
	//TODO scale this right. currently its just (int) [0-255]
	batteryVoltage = analogRead(BATTERY_STATUS_PIN);
	return batteryVoltage;
}

int Hardware::getDistance()
{
	if (millis() - lastDistanceMeasuremeTime >= 50)
	{
		//Serial.print("udaljenost: ");
		int startTime = millis();
		int distance = ultrasonic.ping_cm();
		/*Serial.print(millis() - lastDistanceMeasuremeTime);
		Serial.print(" ");
		Serial.print(millis() - startTime);
		Serial.print(" ");*/
		//Serial.println(ultrasonic.ping_cm());
		lastDistanceMeasuremeTime = millis();
		return distance;
	}
	else
	{
		return -1;
	}
}

int Hardware::servoInit()
{
	Serial.println("INICIJALIZIRAM!!!!!!");
	//R1
	servos[0].attach(50);
	servos[1].attach(51);
	servos[2].attach(52);
	//R2
	servos[3].attach(46);
	servos[4].attach(47);
	servos[5].attach(48);
	//R3
	servos[6].attach(42);
	servos[7].attach(43);
	servos[8].attach(44);
	//L1
	servos[9].attach(22);
	servos[10].attach(23);
	servos[11].attach(24);
	//L2
	servos[12].attach(26);
	servos[13].attach(27);
	servos[14].attach(28);
	//L3
	servos[15].attach(30);
	servos[16].attach(31);
	servos[17].attach(32);

	return 0;
}

int Hardware::servoWrite(float servosNew[18])
{
	int servoOffset = -7; //dictated by servo nature - should be removed in the future - gets calibrated
	//R1
	servos[0].write(servoOffset + calibrationOffsets[0] + mapf(servosNew[0] * (180 / PI), 90, -90, 0, 180));
	servos[1].write(servoOffset + calibrationOffsets[1] + mapf(servosNew[1] * (180 / PI), 90, -90, 0, 180));
	servos[2].write(servoOffset + calibrationOffsets[2] + mapf(servosNew[2] * (180 / PI), -180, 0, 0, 180));
	//R2
	servos[3].write(servoOffset + calibrationOffsets[3] + mapf(servosNew[3] * (180 / PI), 90, -90, 0, 180));
	servos[4].write(servoOffset + calibrationOffsets[4] + mapf(servosNew[4] * (180 / PI), 90, -90, 0, 180));
	servos[5].write(servoOffset + calibrationOffsets[5] + mapf(servosNew[5] * (180 / PI), -180, 0, 0, 180));
	//R3
	servos[6].write(servoOffset + calibrationOffsets[6] + mapf(servosNew[6] * (180 / PI), 90, -90, 0, 180));
	servos[7].write(servoOffset + calibrationOffsets[7] + mapf(servosNew[7] * (180 / PI), 90, -90, 0, 180));
	servos[8].write(servoOffset + calibrationOffsets[8] + mapf(servosNew[8] * (180 / PI), -180, 0, 0, 180));
	//L1
	servos[9].write(servoOffset + calibrationOffsets[9] + mapf(servosNew[9] * (180 / PI), 90, -90, 0, 180));
	servos[10].write(servoOffset + calibrationOffsets[10] + mapf(servosNew[10] * (180 / PI), 90, -90, 180, 0));
	servos[11].write(servoOffset + calibrationOffsets[11] + mapf(servosNew[11] * (180 / PI), -180, 0, 180, 0));
	//L2
	servos[12].write(servoOffset + calibrationOffsets[12] + mapf(servosNew[12] * (180 / PI), 90, -90, 0, 180));
	servos[13].write(servoOffset + calibrationOffsets[13] + mapf(servosNew[13] * (180 / PI), 90, -90, 180, 0));
	servos[14].write(servoOffset + calibrationOffsets[14] + mapf(servosNew[14] * (180 / PI), -180, 0, 180, 0));
	//L3
	servos[15].write(servoOffset + calibrationOffsets[15] + mapf(servosNew[15] * (180 / PI), 90, -90, 0, 180));
	servos[16].write(servoOffset + calibrationOffsets[16] + mapf(servosNew[16] * (180 / PI), 90, -90, 180, 0));
	servos[17].write(servoOffset + calibrationOffsets[17] + mapf(servosNew[17] * (180 / PI), -180, 0, 180, 0));

	return 0;
}

void Hardware::setCalibration(float linData[18])
{
	for (int i = 0; i < 18; i++)
	{
		calibrationOffsets[i] = -linData[i];
	}
}

boolean ByteArrayCompare(byte a[], const char b[], int array_size)
{
	for (int i = 0; i < array_size; ++i)
		if (a[i] != b[i])
			return(false);
	return(true);
}

void Hardware::wifiRead()
{
	byte bytes[30]; //first destructively "P" and then peek "K" i "G"
	if (Serial1.available() >= 22)
	{
		bool PktFound = 0;
		while (Serial1.available() >= 22 && !PktFound)
		{
			Serial.print("sa ");
			Serial.println(Serial1.available());
			bytes[0] = Serial1.read();
			if (bytes[0] == 'P')
			{
				Serial.println("P");
				bytes[1] = Serial1.peek();
				if (bytes[1] == 'K')
				{
					Serial.println("K");
					bytes[1] = Serial1.read();
					bytes[2] = Serial1.peek();
					if (bytes[2] == 'T')
					{
						Serial.println("T");
						bytes[2] = Serial1.read();
						PktFound = 1;
						Serial.println("pass PKT");
					}
				}
			}
			else if (bytes[0] == 'L')
			{
				Serial.println("L");
				bytes[1] = Serial1.peek();
				if (bytes[1] == 'I')
				{
					Serial.println("I");
					bytes[1] = Serial1.read();
					bytes[2] = Serial1.peek();
					if (bytes[2] == 'N')
					{
						Serial.println("N");
						bytes[2] = Serial1.read();
						PktFound = 1;
						Serial.println("pass LIN");
					}
				}
			}
		}
		if (ByteArrayCompare(bytes, "PKT", 3))
		{
			Serial1.readBytes(bytes, 19);
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
			Serial1.readBytes(bytes, 18);
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