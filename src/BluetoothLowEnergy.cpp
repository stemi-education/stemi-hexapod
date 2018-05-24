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


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2904.h>
#include "BluetoothLowEnergy.h"
#include "SharedData.h"

class int8Callback : public BLECharacteristicCallbacks {
private:
	int8_t* data;
public:
	int8Callback(int8_t* input) {
		data = input;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		*data = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class uint8Callback : public BLECharacteristicCallbacks {
private:
	uint8_t* data;
public:
	uint8Callback(uint8_t* input) {
		data = input;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		*data = uint8_t(pCharacteristic->getValue().c_str()[0]);
		//duration of the command
		robot.btInputData.moveDuration = INT8_MAX;
	}
};

class uint8ArrayCallback : public BLECharacteristicCallbacks {
private:
	uint8_t* data;
	uint8_t lenght;
public:
	uint8ArrayCallback(uint8_t* input, uint8_t arrayLen) {
		data = input;
		lenght = arrayLen;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		for (int i = 0; i < lenght; i++) {
		data[i] = uint8_t(pCharacteristic->getValue().c_str()[i]);
		}
	}
};

class int16Callback : public BLECharacteristicCallbacks {
private:
	int16_t* data;
public:
	int16Callback(int16_t* input) {
		data = input;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		*data = int16_t(pCharacteristic->getValue().c_str()[0]) + int16_t(pCharacteristic->getValue().c_str()[1] << 8);
	}
};


BluetoothLowEnergy::BluetoothLowEnergy(std::string deviceName) {
	createBLEDevice(deviceName);
	createBLEServer();
	createMovementServiceWithCharacteristics();
	createPoseServiceWithCharacteristics();
	createLEDServiceWithCharacteristics();
	createParameterServiceWithCharacteristics();
	createBatteryServiceWithCharacteristics();
	startAdvertising();
}

void BluetoothLowEnergy::createBLEDevice(std::string deviceName) {
	BLEDevice::init(deviceName);
	// Ako zelimo dodati neku fun da bude adresa svakog hexaca drugacija
};
	
void BluetoothLowEnergy::createBLEServer() {
	server = BLEDevice::createServer();
};
	
void BluetoothLowEnergy::startAdvertising() {
	advertising = server->getAdvertising();
	advertising->start();
};

BLE2904* BluetoothLowEnergy::createBLE2904Descriptor(uint8_t format, uint16_t unit) {
	BLE2904* descriptor = new BLE2904();
	descriptor->setFormat(format);
	descriptor->setNamespace(1);
	descriptor->setUnit(unit);
	return descriptor;
};

void BluetoothLowEnergy::createMovementServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	movementService = server->createService(MOVEMENT_SERVICE_UUID);

	BLECharacteristic* linearVelocityCharacteristic = movementService->createCharacteristic(
		LINEARVELOCITY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//linearVelocityCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_UINT8, 0x27ad));

	BLECharacteristic* directionCharacteristic = movementService->createCharacteristic(
		DIRECTION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//directionCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT16, 0x2763));

	BLECharacteristic* angularVelocityCharacteristic = movementService->createCharacteristic(
		ANGULARVELOCITY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//angularVelocityCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));

	linearVelocityCharacteristic->setValue(&init_data[0], 1);
	directionCharacteristic->setValue(init_data, 2);
	angularVelocityCharacteristic->setValue(&init_data[0], 1);


	linearVelocityCharacteristic->setCallbacks(new uint8Callback(&robot.btInputData.linearVelocity));
	directionCharacteristic->setCallbacks(new int16Callback(&robot.btInputData.direction));
	angularVelocityCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.angularVelocity));
	
	movementService->start();
};
	
void BluetoothLowEnergy::createPoseServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	poseService = server->createService(POSE_SERVICE_UUID);
	
	BLECharacteristic* translationXCharacteristic = poseService->createCharacteristic(
		TRANSLATIONX_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//translationXCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));

	BLECharacteristic* translationYCharacteristic = poseService->createCharacteristic(
		TRANSLATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//translationYCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));

	BLECharacteristic* translationZCharacteristic = poseService->createCharacteristic(
		TRANSLATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//translationZCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));
	
	BLECharacteristic* rotationXCharacteristic = poseService->createCharacteristic(
		ROTATIONX_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//rotationXCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));
	
	BLECharacteristic* rotationYCharacteristic = poseService->createCharacteristic(
		ROTATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//rotationYCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));
	

	BLECharacteristic* rotationZCharacteristic = poseService->createCharacteristic(
		ROTATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	//rotationZCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_SINT8, 0x27ad));

	translationXCharacteristic->setValue(&init_data[0], 1);
	translationYCharacteristic->setValue(&init_data[0], 1);
	translationZCharacteristic->setValue(&init_data[0], 1);
	rotationXCharacteristic->setValue(&init_data[0], 1);
	rotationYCharacteristic->setValue(&init_data[0], 1);
	rotationZCharacteristic->setValue(&init_data[0], 1);

	translationXCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.translationX));
	translationYCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.translationY));
	translationZCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.translationZ));
	rotationXCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.rotationX));
	rotationYCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.rotationY));
	rotationZCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.rotationZ));

	poseService->start();
};

void BluetoothLowEnergy::createParameterServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	parameterService = server->createService(PARAMETER_SERVICE);

	BLECharacteristic* modeCharacteristic = parameterService->createCharacteristic(
		MODE_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* gaitIDCharacteristic = parameterService->createCharacteristic(
		GAITID_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* userSliderCharacteristic = parameterService->createCharacteristic(
		USERSLIDER_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* versionCharacteristic = parameterService->createCharacteristic(
		VERSION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ
	);

	modeCharacteristic->setValue(&init_data[0], 1);
	gaitIDCharacteristic->setValue(&init_data[0], 1);
	userSliderCharacteristic->setValue(&init_data[0], 1);
	versionCharacteristic->setValue(robot.version, 3);

	modeCharacteristic->setCallbacks(new int8Callback(&robot.mode));
	gaitIDCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.gaitID));
	userSliderCharacteristic->setCallbacks(new int8Callback(&robot.userSlider));

	parameterService->start();
};

void BluetoothLowEnergy::createLEDServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	LEDService = server->createService(LED_SERVICE);

	BLECharacteristic* directionCharacteristic = LEDService->createCharacteristic(
		LEDDIRECTION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* spreadRatioCharacteristic = LEDService->createCharacteristic(
		SPREADRATIO_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* primatyClrCharacteristic = LEDService->createCharacteristic(
		PRIMARYCLR_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* secundaryClrCharacteristic = LEDService->createCharacteristic(
		SECUNDARYCLR_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* rotaiotnSpeedCharacteristic = LEDService->createCharacteristic(
		ROTATIONSPEED_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* blinkingSpeedCharacteristic = LEDService->createCharacteristic(
		BLINKINGSPEED_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);


	directionCharacteristic->setValue(&init_data[0], 2);
	spreadRatioCharacteristic->setValue(&init_data[0], 1);
	primatyClrCharacteristic->setValue(&init_data[0], 3);
	secundaryClrCharacteristic->setValue(&init_data[0], 3);
	rotaiotnSpeedCharacteristic->setValue(&init_data[0], 2);
	blinkingSpeedCharacteristic->setValue(&init_data[0], 1);


	directionCharacteristic->setCallbacks(new int16Callback(&robot.btInputData.ledDiretion));
	spreadRatioCharacteristic->setCallbacks(new uint8Callback(&robot.btInputData.ledSpreadRatio));
	primatyClrCharacteristic->setCallbacks(new uint8ArrayCallback(robot.btInputData.ledPrimarClr, 3));
	secundaryClrCharacteristic->setCallbacks(new uint8ArrayCallback(robot.btInputData.ledSecondarClr, 3));
	rotaiotnSpeedCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.ledRotationSpeed));
	blinkingSpeedCharacteristic->setCallbacks(new uint8Callback(&robot.btInputData.ledBlinkingSpeed));

	LEDService->start();
};

void BluetoothLowEnergy::createBatteryServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	batteryService = server->createService(BATTERY_SERVICE_UUID);

	BLECharacteristic* batteryLevelCharacteristic = batteryService->createCharacteristic(
		BATTERYLEVEL_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ | 
		BLECharacteristic::PROPERTY_WRITE);
	//batteryLevelCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_UINT8, 0x27ad));
	
	batteryLevelCharacteristic->setValue(&init_data[0], 1);

	batteryService->start();
};
