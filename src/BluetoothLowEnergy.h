/*

This file is part of STEMI hexapod library.

Copyright (C) 2017 STEMI d.o.o.
Authors: Antun Vukicevic, et al.
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


#ifndef _BLUETOOTHLOWENERGY_h
#define _BLUETOOTHLOWENERGY_h

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2904.h>

#include "SharedData.h"

#define MOVEMENT_SERVICE_UUID "5fe3a3ac-bc0d-4484-aad5-db8865fec4c7"
#define LINEARVELOCITY_CHARACTERISTIC_UUID "3ed49873-e0c0-4a7a-9d69-1d533cea05b7"
#define DIRECTION_CHARACTERISTIC_UUID "3db7ef30-b289-4ef0-8dc8-adac1ef811db"
#define ANGULARVELOCITY_CHARACTERISTIC_UUID "02173f46-7417-4238-8975-e2d6b557309c"
#define COMMANDTIMER_CHARACTERISTIC_UUD "aeac4046-adfb-498f-ad1e-a9ec96771a18"

#define POSE_SERVICE_UUID "e2b86f75-dbfc-47d0-9ea0-98dd2cb5fe63"
#define TRANSLATIONX_CHARACTERISTIC_UUID "29770801-3c1e-44de-987c-8ec1dd31a860"
#define TRANSLATIONY_CHARACTERISTIC_UUID "d12dfb34-95c5-444c-89ff-3dc7c43df1f4"
#define TRANSLATIONZ_CHARACTERISTIC_UUID "3790e451-5da6-4a00-b172-6ec0e627f84f"
#define ROTATIONX_CHARACTERISTIC_UUID "fa7fd7a7-b183-411c-a1e9-bfd26ac27428"
#define ROTATIONY_CHARACTERISTIC_UUID "0ed7370a-6188-4072-b5f2-7d9250ec8c3c"
#define ROTATIONZ_CHARACTERISTIC_UUID "79cf5c3a-6df8-402c-9ba0-607582eb8b62"

#define PARAMETER_SERVICE "6ee32c61-cb9c-4471-a79d-2a2a98afd49a"
#define MODE_CHARACTERISTIC_UUID "25017253-ae08-4681-ba9b-c6c0bf85da36"
#define GAITID_CHARACTERISTIC_UUID "be930e0b-db94-48de-bdde-6e50fdac6fa0"
#define UNIVERSAL_DATA_CHARACTERISTIC_UUID "387551c9-f9e9-4e06-804b-e167bcd520ea"
#define USERSLIDER_CHARACTERISTIC_UUID	"a7b2097f-0fd6-4e14-b745-0c7e9fc3214b"
#define SOFTWARE_VERSION_CHARACTERISTIC_UUID "c526c005-97cd-46ca-992e-dae2b83c36e9"
#define HARDWARE_VERSION_CHARACTERISTIC_UUID "2bbf68c9-2d73-4620-8eba-1758f6bef1b0"
#define STEPHEIGHT_CHARACTERISTIC_UUID "e4fe2831-f7ae-4148-afd5-154a47f41530"
#define NAME_CHARACTERISTIC_UUID "09769166-195f-495e-baa9-383c85211e97"

#define LED_SERVICE "146d53ec-a92c-452c-9c2f-99bd7a6fbf8d"
#define LEDDIRECTION_CHARACTERISTIC_UUID "fcd825b7-d05c-4829-b0e0-3b0fa8b4caff"
#define SPREADRATIO_CHARACTERISTIC_UUID "6471f93b-f7b1-4464-b121-ecda42348d52"
#define PRIMARYCLR_CHARACTERISTIC_UUID "f21d9bb4-07fb-44cc-bc69-f6db5aba834a"
#define SECUNDARYCLR_CHARACTERISTIC_UUID "1e1df361-4b60-4758-987e-51c6fd8463fd"
#define ROTATIONSPEED_CHARACTERISTIC_UUID "f7cae648-0243-443c-ab53-47a61cd0180e"
#define BLINKINGSPEED_CHARACTERISTIC_UUID "174b1b6d-6ac0-4b38-8751-8d027bd325f6"

#define BATTERY_SERVICE_UUID "0000180f-0000-1000-8000-00805f9b34fb"
#define BATTERYLEVEL_CHARACTERISTIC_UUID "00002a19-0000-1000-8000-00805f9b34fb"

#define BATCH_SERVICE_UUID "651707bb-61f7-4338-b5a9-c36e02c8e2f1"
#define BATCH_CHARACTERISTIC_UUID "da86113b-ae65-472e-ac2e-ad7d8a00feac"

class BluetoothLowEnergy {
public:
	BluetoothLowEnergy(std::string deviceName);

	BLEServer* server;
	
	BLEService* movementService;
	BLEService* poseService;
	BLEService* parameterService;
	BLEService* LEDService;
	BLEService* batteryService;
	BLEService* nameService;
	BLEService* batchService;
	BLEService* universalDataService;
	
	BLEAdvertising* advertising;

private:
	void createBLEDevice(std::string serverName);
	void createBLEServer();
	void startAdvertising();

	BLE2904* createBLE2904Descriptor(uint8_t format, uint16_t unit);

	void createMovementServiceWithCharacteristics();
	void createPoseServiceWithCharacteristics();
	void createParameterServiceWithCharacteristics();
	void createLEDServiceWithCharacteristics();
	void createBatteryServiceWithCharacteristics();
	void createBatchMovementServiceWithCharacteristic();
};

#endif