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
#include <BLE2902.h>
#include "BluetoothLowEnergy.h"
#include "SharedData.h"
#include "esp_ota_ops.h"

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

class int8ArrayCallback : public BLECharacteristicCallbacks {
private:
	int8_t* data;
	uint8_t lenght;
public:
	int8ArrayCallback(int8_t* input, uint8_t arrayLen) {
		data = input;
		lenght = arrayLen;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		for (int i = 0; i < lenght; i++) {
			data[i] = int8_t(pCharacteristic->getValue().c_str()[i]);
		}
	}
};

class robotNameCallback : public BLECharacteristicCallbacks {
private:
public:
	robotNameCallback() {
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		robot.storeName(pCharacteristic->getValue());
		char nameDummy[20];
		strcpy(nameDummy, pCharacteristic->getValue().c_str());
		pCharacteristic->setValue((uint8_t*)nameDummy, 20);
	}
};

class robotActionNameCallback : public BLECharacteristicCallbacks {
private:
public:
	robotActionNameCallback() {
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
			robot.startAction(pCharacteristic->getValue());
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

class otaCallback : public BLECharacteristicCallbacks {
private:
	esp_ota_handle_t otaHandler = 0;
	bool updateFlag = false;
	bool readyFlag = false;
	int bytesReceived = 0;
	int timesWritten = 0;
public:
	otaCallback() {
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		std::string rxData = pCharacteristic->getValue();
		if (!updateFlag) { //If it's the first packet of OTA since bootup, begin OTA
			Serial.println("BeginOTA");
			esp_ota_begin(esp_ota_get_next_update_partition(NULL), OTA_SIZE_UNKNOWN, &otaHandler);
			updateFlag = true;
			bytesReceived = 0;
		}
		if (rxData.length() > 0)
		{
			esp_ota_write(otaHandler, rxData.c_str(), rxData.length());
			bytesReceived += rxData.length();
			if (rxData.length() != FULL_PACKET)
			{
				esp_ota_end(otaHandler);
				Serial.println("EndOTA");
				if (ESP_OK == esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL))) {
				delay(2000);
				esp_restart();
				}
				else {
				Serial.println("Upload Error");
				}
			}
		}
		Serial.println(bytesReceived);

		uint8_t txData[5] = {1, 2, 3, 4, 5};
		pCharacteristic->setValue((uint8_t*)txData, 5);
		pCharacteristic->notify();
	}
};

class nameCallback : public BLECharacteristicCallbacks {
public:
	nameCallback() {
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		char nameDummy[20];
		strcpy(nameDummy, robot.name.c_str());
		char robotName[50] = "";
		int robotNameLength = 0;
		for (int i = 0; true; i++) {
			robotName[i] = nameDummy[i];
			robotNameLength += 1;
			if (nameDummy[i] == '\0') {
			break;
			}
		}
		pCharacteristic->setValue((uint8_t*)robotName, robotNameLength);
		pCharacteristic->notify();
	}
};



BluetoothLowEnergy::BluetoothLowEnergy(std::string deviceName) {
	deviceName = deviceName.substr(0, 29);
	createBLEDevice(deviceName);
	createBLEServer();
	createMovementServiceWithCharacteristics();
	createPoseServiceWithCharacteristics();
	createLEDServiceWithCharacteristics();
	createParameterServiceWithCharacteristics();
	createBatteryServiceWithCharacteristics();
	createBatchMovementServiceWithCharacteristic();
	startAdvertising();
}

void BluetoothLowEnergy::createBLEDevice(std::string deviceName) {
	BLEDevice::init(deviceName);
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
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* directionCharacteristic = movementService->createCharacteristic(
		DIRECTION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* angularVelocityCharacteristic = movementService->createCharacteristic(
		ANGULARVELOCITY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

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
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* translationYCharacteristic = poseService->createCharacteristic(
		TRANSLATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* translationZCharacteristic = poseService->createCharacteristic(
		TRANSLATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* rotationXCharacteristic = poseService->createCharacteristic(
		ROTATIONX_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);
	
	BLECharacteristic* rotationYCharacteristic = poseService->createCharacteristic(
		ROTATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* rotationZCharacteristic = poseService->createCharacteristic(
		ROTATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

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
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_NOTIFY
	);

	BLECharacteristic* gaitIDCharacteristic = parameterService->createCharacteristic(
		GAITID_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* universalDataCharacteristic = parameterService->createCharacteristic(
		UNIVERSAL_DATA_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* userSliderCharacteristic = parameterService->createCharacteristic(
		USERSLIDER_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* softwareVersionCharacteristic = parameterService->createCharacteristic(
		SOFTWARE_VERSION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ
	);

	BLECharacteristic* hardwareVersionCharacteristic = parameterService->createCharacteristic(
		HARDWARE_VERSION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ
	);

	modeCharacteristic->setValue(&init_data[0], 1);
	gaitIDCharacteristic->setValue(&init_data[0], 1);
	universalDataCharacteristic->setValue(init_data, 1);
	userSliderCharacteristic->setValue(&init_data[0], 1);
	softwareVersionCharacteristic->setValue(robot.hexSwVersion, 3);
	hardwareVersionCharacteristic->setValue(robot.hexHwVersion, 3);

	modeCharacteristic->setCallbacks(new int8Callback(&robot.mode));
	gaitIDCharacteristic->setCallbacks(new int8Callback(&robot.btInputData.gaitID));
	universalDataCharacteristic->setCallbacks(new int8Callback(&robot.universalData[0]));
	userSliderCharacteristic->setCallbacks(new int8Callback(&robot.userSlider));

	parameterService->start();
};

void BluetoothLowEnergy::createLEDServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	LEDService = server->createService(LED_SERVICE);

	BLECharacteristic* directionCharacteristic = LEDService->createCharacteristic(
		LEDDIRECTION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* spreadRatioCharacteristic = LEDService->createCharacteristic(
		SPREADRATIO_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* primatyClrCharacteristic = LEDService->createCharacteristic(
		PRIMARYCLR_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* secundaryClrCharacteristic = LEDService->createCharacteristic(
		SECUNDARYCLR_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* rotaiotnSpeedCharacteristic = LEDService->createCharacteristic(
		ROTATIONSPEED_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
	);

	BLECharacteristic* blinkingSpeedCharacteristic = LEDService->createCharacteristic(
		BLINKINGSPEED_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR
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
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_NOTIFY);
	
	batteryLevelCharacteristic->setValue(&init_data[0], 1);

	batteryService->start();
};

class batchCallback : public BLECharacteristicCallbacks {
public:

	SharedData * data;

	batchCallback(SharedData* dataNew) {
		data = dataNew;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		data->btInputData.linearVelocity = uint8_t(pCharacteristic->getValue().c_str()[0]);
		data->btInputData.direction = int16_t(pCharacteristic->getValue().c_str()[1]) + int16_t(pCharacteristic->getValue().c_str()[2] << 8);
		data->btInputData.angularVelocity = int8_t(pCharacteristic->getValue().c_str()[3]);
		data->btInputData.translationX = int8_t(pCharacteristic->getValue().c_str()[4]);
		data->btInputData.translationY = int8_t(pCharacteristic->getValue().c_str()[5]);
		data->btInputData.translationZ = int8_t(pCharacteristic->getValue().c_str()[6]);
		data->btInputData.rotationX = int8_t(pCharacteristic->getValue().c_str()[7]);
		data->btInputData.rotationY = int8_t(pCharacteristic->getValue().c_str()[8]);
		data->btInputData.rotationZ = int8_t(pCharacteristic->getValue().c_str()[9]);
		data->userSlider = int8_t(pCharacteristic->getValue().c_str()[10]);
		data->btInputData.ledDiretion = int16_t(pCharacteristic->getValue().c_str()[11]) + int16_t(pCharacteristic->getValue().c_str()[12] << 8);
		data->btInputData.ledSpreadRatio = uint8_t(pCharacteristic->getValue().c_str()[13]);
		for (int i = 0; i < 3; i++) {
		  data->btInputData.ledPrimarClr[i] = uint8_t(pCharacteristic->getValue().c_str()[14 + i]);
		}
		for (int i = 0; i < 3; i++) {
		  data->btInputData.ledSecondarClr[i] = uint8_t(pCharacteristic->getValue().c_str()[17 + i]);
		}
		data->btInputData.ledRotationSpeed = int8_t(pCharacteristic->getValue().c_str()[20]);
		data->btInputData.ledBlinkingSpeed = uint8_t(pCharacteristic->getValue().c_str()[21]);
	}
};

void BluetoothLowEnergy::createBatchMovementServiceWithCharacteristic() {
	char nameDummy[20];
	strcpy(nameDummy, robot.name.c_str());
    char robotName[50] = "";
	int robotNameLength = 0;
    for (int i = 0; true; i++) {
        robotName[i] = nameDummy[i];
		robotNameLength += 1;
        if (nameDummy[i] == '\0') {
          break;
        }
    }

	batchService = server->createService(BATCH_SERVICE_UUID);
	BLECharacteristic* batchCharacteristic = batchService->createCharacteristic(
		BATCH_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_WRITE_NR);
	
	BLECharacteristic* actionNameCharacteristic = batchService->createCharacteristic(
		ACTION_NAME_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_INDICATE);

	BLECharacteristic* pOtaCharacteristic = batchService->createCharacteristic(
        OTA_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE
    );

	BLECharacteristic* nameCharacteristic = batchService->createCharacteristic(
		NAME_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE |
		BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE);

	nameCharacteristic->setValue((uint8_t*)robotName, robotNameLength);
  	nameCharacteristic->setCallbacks(new nameCallback());

	pOtaCharacteristic->addDescriptor(new BLEDescriptor((uint16_t)0x2902));
  	pOtaCharacteristic->setCallbacks(new otaCallback());

	uint8_t batchCommands[22];
	batchCharacteristic->setValue(&batchCommands[0], 22);
	actionNameCharacteristic->setValue("");

	batchCharacteristic->setCallbacks(new batchCallback(&robot));
	actionNameCharacteristic->setCallbacks(new robotActionNameCallback());
	batchService->start();
}
