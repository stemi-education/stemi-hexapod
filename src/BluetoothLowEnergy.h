#ifndef _BLUETOOTHLOWENERGY_h
#define _BLUETOOTHLOWENERGY_h

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
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
#define STEPHEIGHT_CHARACTERISTIC_UUID "e4fe2831-f7ae-4148-afd5-154a47f41530"

#define BATTERY_SERVICE_UUID "0000180f-0000-1000-8000-00805f9b34fb"
#define BATTERYLEVEL_CHARACTERISTIC_UUID "00002a19-0000-1000-8000-00805f9b34fb"


class BluetoothLowEnergy {
public:

	BluetoothLowEnergy(std::string deviceName, SharedData* sharedData);

	BLEServer* server;
	BLEService* movementService;
	BLEService* poseService;
	
	BLEService* parameterService;
	BLEService* batteryService;
	BLEAdvertising* advertising;

	SharedData* sharedData;

	void createBLEDevice(std::string serverName);
	void createBLEServer();
	void createMovementServiceWithCharacteristics();
	//void createPoseServiceWithCharacteristics();
	//void createParameterServiceWithCharacteristics();
	void createBatteryServiceWithCharacteristics();
	void startAdvertising();
};

class LinearVelocityCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shData;
public:
	LinearVelocityCallback(SharedData* data) {
		shData = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shData->btCtrl.linearVelocity = (float)(uint8_t(pCharacteristic->getValue().c_str()[0]) / 10.0); // Reciving data in interval [0, 100], maping data to interval [0, 10]
	}
};

class DirectionCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shData;
public:
	DirectionCallback(SharedData* data) {
		shData = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		int16_t recived = int16_t(pCharacteristic->getValue().c_str()[0]) + int16_t(pCharacteristic->getValue().c_str()[1] << 8); // Reciving data in interval [-180, 180]
		shData->btCtrl.direction = (float)(recived * PI / 180 + PI / 2); // Maping data to interval [0, 2PI]
	}
};

class AngularVelocityCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shData;
public:
	AngularVelocityCallback(SharedData* data) {
		shData = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shData->btCtrl.angularVelocity = (float)(int8_t(pCharacteristic->getValue().c_str()[0]) / 200.0); // Reciving data in interval [-100, 100], maping data to interval [-0.5, 0.5]
	}
};

/*
class CommandTimerCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	CommandTimerCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->commandTimer = int16_t(pCharacteristic->getValue().c_str()[0]) + int16_t(pCharacteristic->getValue().c_str()[1] << 8);
	}
};

class TranslationXCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	TranslationXCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->translationX = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class TranslationYCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	TranslationYCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->translationY = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class TranslationZCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	TranslationZCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->translationZ = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class RotationXCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	RotationXCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->rotationX = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class RotationYCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	RotationYCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->rotationY = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};

class RotationZCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shdata;
public:
	RotationZCallback(SharedData* data) {
		shdata = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		shdata->rotationZ = int8_t(pCharacteristic->getValue().c_str()[0]);
	}
};
*/

class BatteryLevelCallback : public BLECharacteristicCallbacks {
private:
	SharedData* shData;
public:
	BatteryLevelCallback(SharedData* data) {
		shData = data;
	}
	void onWrite(BLECharacteristic* pCharacteristic) {
		//shdata->batteryLevel = uint8_t(pCharacteristic->getValue().c_str()[0]);
	}
};


#endif