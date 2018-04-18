#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2904.h>
#include "BluetoothLowEnergy.h"

BluetoothLowEnergy::BluetoothLowEnergy(std::string deviceName, SharedData* data) {
	sharedData = data;
	createBLEDevice(deviceName);
	createBLEServer();
	createMovementServiceWithCharacteristics();
	//createPoseServiceWithCharacteristics();
	//createParameterServiceWithCharacteristics();
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
	
void BluetoothLowEnergy::createMovementServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	movementService = server->createService(MOVEMENT_SERVICE_UUID);

	BLECharacteristic* linearVelocityCharacteristic = movementService->createCharacteristic(
		LINEARVELOCITY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	
	BLE2904* linearVelocityDescriptor = new BLE2904();
	linearVelocityDescriptor->setFormat(BLE2904::FORMAT_UINT8);
	linearVelocityDescriptor->setNamespace(1);
	linearVelocityDescriptor->setUnit(0x27ad);

	linearVelocityCharacteristic->addDescriptor(linearVelocityDescriptor);

	BLECharacteristic* directionCharacteristic = movementService->createCharacteristic(
		DIRECTION_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLE2904* directionDescriptor = new BLE2904();
	directionDescriptor->setFormat(BLE2904::FORMAT_SINT16);
	directionDescriptor->setNamespace(1);
	directionDescriptor->setUnit(0x2763);

	directionCharacteristic->addDescriptor(directionDescriptor);

	BLECharacteristic* angularVelocityCharacteristic = movementService->createCharacteristic(
		ANGULARVELOCITY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLE2904* angularVelocityDescriptor = new BLE2904();
	angularVelocityDescriptor->setFormat(BLE2904::FORMAT_SINT8);
	angularVelocityDescriptor->setNamespace(1);
	angularVelocityDescriptor->setUnit(0x27ad);

	angularVelocityCharacteristic->addDescriptor(angularVelocityDescriptor);

	BLECharacteristic* commandTimerCharacteristic = movementService->createCharacteristic(
		COMMANDTIMER_CHARACTERISTIC_UUD,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);
	
	linearVelocityCharacteristic->setValue(&init_data[0], 1);
	directionCharacteristic->setValue(init_data, 2);
	angularVelocityCharacteristic->setValue(&init_data[0], 1);
	init_data[0] = 0xFF;
	init_data[1] = 0xFF;
	commandTimerCharacteristic->setValue(init_data, 2);

	linearVelocityCharacteristic->setCallbacks(new LinearVelocityCallback(sharedData));
	directionCharacteristic->setCallbacks(new DirectionCallback(sharedData));
	angularVelocityCharacteristic->setCallbacks(new AngularVelocityCallback(sharedData));
	//commandTimerCharacteristic->setCallbacks(new CommandTimerCallback(sharedData));
	
	movementService->start();
};

/*	
void BluetoothLowEnergy::createPoseServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	poseService = server->createService(POSE_SERVICE_UUID);

	BLECharacteristic* translationXCharacteristic = poseService->createCharacteristic(
		TRANSLATIONX_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* translationYCharacteristic = poseService->createCharacteristic(
		TRANSLATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* translationZCharacteristic = poseService->createCharacteristic(
		TRANSLATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* rotationXCharacteristic = poseService->createCharacteristic(
		ROTATIONX_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* rotationYCharacteristic = poseService->createCharacteristic(
		ROTATIONY_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	BLECharacteristic* rotationZCharacteristic = poseService->createCharacteristic(
		ROTATIONZ_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
	);

	translationXCharacteristic->setValue(&init_data[0], 1);
	translationYCharacteristic->setValue(&init_data[0], 1);
	translationZCharacteristic->setValue(&init_data[0], 1);
	rotationXCharacteristic->setValue(&init_data[0], 1);
	rotationYCharacteristic->setValue(&init_data[0], 1);
	rotationZCharacteristic->setValue(&init_data[0], 1);

	translationXCharacteristic->setCallbacks(new TranslationXCallback(data));
	translationYCharacteristic->setCallbacks(new TranslationYCallback(data));
	translationZCharacteristic->setCallbacks(new TranslationZCallback(data));
	rotationXCharacteristic->setCallbacks(new RotationXCallback(data));
	rotationYCharacteristic->setCallbacks(new RotationYCallback(data));
	rotationZCharacteristic->setCallbacks(new RotationZCallback(data));

	poseService->start();

};

void BluetoothLowEnergy::createParameterServiceWithCharacteristics() {
	parameterService = server->createService(PARAMETER_SERVICE);
};
*/

void BluetoothLowEnergy::createBatteryServiceWithCharacteristics() {
	uint8_t init_data[2] = { 0, 0 };
	batteryService = server->createService(BATTERY_SERVICE_UUID);

	BLECharacteristic* batteryLevelCharacteristic = batteryService->createCharacteristic(
		BATTERYLEVEL_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ);

	BLE2904* batteryLevelDescriptor = new BLE2904();
	batteryLevelDescriptor->setFormat(BLE2904::FORMAT_UINT8);
	batteryLevelDescriptor->setNamespace(1);
	batteryLevelDescriptor->setUnit(0x27ad);
	
	batteryLevelCharacteristic->addDescriptor(batteryLevelDescriptor);
	
	batteryLevelCharacteristic->setValue(&init_data[0], 1);

	batteryLevelCharacteristic->setCallbacks(new BatteryLevelCallback(sharedData));

	batteryService->start();
};

void BluetoothLowEnergy::startAdvertising() {
	advertising = server->getAdvertising();
	advertising->start();
};