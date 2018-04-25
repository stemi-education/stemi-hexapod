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


	linearVelocityCharacteristic->setCallbacks(new uint8Callback(&robot.btCtrl.linearVelocity));
	directionCharacteristic->setCallbacks(new int16Callback(&robot.btCtrl.direction));
	angularVelocityCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.angularVelocity));
	
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

	translationXCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.translationX));
	translationYCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.translationY));
	translationZCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.translationZ));
	rotationXCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.rotationX));
	rotationYCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.rotationY));
	rotationZCharacteristic->setCallbacks(new int8Callback(&robot.btCtrl.rotationZ));

	poseService->start();
};

void BluetoothLowEnergy::createParameterServiceWithCharacteristics() {
	//TODO: Implement parameter service when ready
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


	directionCharacteristic->setCallbacks(new int16Callback(&robot.btCtrl.ledDiretion));
	spreadRatioCharacteristic->setCallbacks(new uint8Callback(&robot.btCtrl.spreadRatio));
	primatyClrCharacteristic->setCallbacks(new uint8ArrayCallback(robot.btCtrl.primaryClr, 3));
	secundaryClrCharacteristic->setCallbacks(new uint8ArrayCallback(robot.btCtrl.secundaryClr, 3));
	rotaiotnSpeedCharacteristic->setCallbacks(new int16Callback(&robot.btCtrl.rotationSpeed));
	blinkingSpeedCharacteristic->setCallbacks(new uint8Callback(&robot.btCtrl.blinkingSpeed));

	LEDService->start();
};

void BluetoothLowEnergy::createBatteryServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	batteryService = server->createService(BATTERY_SERVICE_UUID);

	BLECharacteristic* batteryLevelCharacteristic = batteryService->createCharacteristic(
		BATTERYLEVEL_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ);
	//batteryLevelCharacteristic->addDescriptor(createBLE2904Descriptor(BLE2904::FORMAT_UINT8, 0x27ad));
	
	batteryLevelCharacteristic->setValue(&init_data[0], 1);

	batteryService->start();
};
