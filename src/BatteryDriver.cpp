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


#include "BatteryDriver.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_7;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;


BatteryDriver::BatteryDriver()
{
	preferences.begin("my-app", false);
	batteryPinCalibrationValue = preferences.getFloat("batCalibVal",0);
	Serial.print("batPinCalibVal loaded: ");
	Serial.println(batteryPinCalibrationValue);
}

void BatteryDriver::checkState()
{
	robot.batteryState.voltage = LPFvoltage(readBatteryVoltage());
	float minVoltage = 3.6, maxVoltage = 3.85;
	
	robot.batteryState.percentage = min(max(robot.batteryState.voltage - minVoltage, 0), maxVoltage-minVoltage)/(maxVoltage-minVoltage)*100;
	Serial.print(batteryPinCalibrationValue);
	Serial.print(" ");
	Serial.print(readBatteryVoltage());
	Serial.print(" ");
	Serial.print(robot.batteryState.voltage);
	Serial.print(" ");
	Serial.println(robot.batteryState.percentage);
}

float BatteryDriver::readBatteryVoltage()
{
	float senVal = (float)(analogRead(BATTERY_STATUS_PIN));
	return  (-0.000000000023926 * pow(senVal, 3) + 0.000000094746 * pow(senVal, 2) + 0.00074539 * senVal + 0.14925) * 2.0 + batteryPinCalibrationValue;
}

float BatteryDriver::LPFvoltage(float valueNew)
{
	float alpha = 0.99;
	return alpha * robot.batteryState.voltage + (1 - alpha)*(valueNew);
}

void BatteryDriver::calibrateBatteryPin()
{
	batteryPinCalibrationValue = 0;
	float batteryPinCalibrationValueSum = 0;
	for(int i = 0; i < 10; i++)
		batteryPinCalibrationValueSum += BATTERY_PIN_CALIBRATION_REF_V - readBatteryVoltage();
	batteryPinCalibrationValue += batteryPinCalibrationValueSum / 10;
	preferences.putFloat("batCalibVal", batteryPinCalibrationValue);
	Serial.print("Stored batCalibVal: ");
	Serial.println(batteryPinCalibrationValue);
}
