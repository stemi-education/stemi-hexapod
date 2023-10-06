
#include "TouchDriver.h"
#include "Arduino.h"

Touch* Touch::instance;

Touch::Touch(int treshold, int interval, int value) {
	instance = this;
	touch_treshold = treshold;
	cycle_counter_detect_interval = interval;
	cycle_counter_detect_value = value;

	touchAttachInterrupt(T4, Touch::touchInterruptCallbackT4, 20);
	touchAttachInterrupt(T6, Touch::touchInterruptCallbackT6, 25);
	touchAttachInterrupt(T7, Touch::touchInterruptCallbackT7, 25);
}

void Touch::touchInterruptCallbackT4() {
	instance->writeInterruptTime(T4);
}

void Touch::touchInterruptCallbackT6() {
	instance->writeInterruptTime(T6);
}

void Touch::touchInterruptCallbackT7() {
	instance->writeInterruptTime(T7);
}

void Touch::writeInterruptTime(int touch) {
	// Check order
	switch (touch) {
		case T4: {
			interruptTime[1] = millis();
			break;
		}
		case T6: {
			interruptTime[0] = millis();
			break;
		}
		case T7: {
			interruptTime[2] = millis();
			break;
		}
	}
}

void Touch::refreshTouchCycleCounter() {
	unsigned long time = millis();

	if (time - interruptTime[0] < cycle_counter_detect_interval) touchCycleCounter[0]++;
	else touchCycleCounter[0] = 0;

	if (time - interruptTime[1] < cycle_counter_detect_interval) touchCycleCounter[1]++;
	else touchCycleCounter[1] = 0;

	if (time - interruptTime[2] < cycle_counter_detect_interval) touchCycleCounter[2]++;
	else touchCycleCounter[2] = 0;

}

int Touch::touchCycleCounterToTouchID() {
	if (touchCycleCounter[0] > cycle_counter_detect_value) {
		if (touchCycleCounter[1] > 0 && touchCycleCounter[2] > 0) return 0x07;
		if (touchCycleCounter[1] > 0) return 0x03;
		if (touchCycleCounter[2] > 0) return 0x05;
		return 0x01;
	}
	if (touchCycleCounter[1] > cycle_counter_detect_value) {
		if (touchCycleCounter[0] > 0 && touchCycleCounter[2] > 0) return 0x07;
		if (touchCycleCounter[0] > 0) return 0x03;
		if (touchCycleCounter[2] > 0) return 0x06;
		return 0x02;
	}
	if (touchCycleCounter[2] > cycle_counter_detect_value) {
		if (touchCycleCounter[0] > 0 && touchCycleCounter[1] > 0) return 0x07;
		if (touchCycleCounter[0] > 0) return 0x05;
		if (touchCycleCounter[1] > 0) return 0x06;
		return 0x04;
	}

	return 0;
}

void Touch::detectTouchPattern(int touchID) {
	if (touchID == 0x00) {
		lastTouchID = 0x00;
		activeTouchID = 0x00;
		touchDetected = false;
	}
	else if (lastTouchID == 0x00) {
		if (touchID != 0x00) {
			lastTouchID = touchID;
			activeTouchID = touchID;
			touchDetected = true;
		}
	}
	else touchDetected = false;
}

void Touch::checkTouch() {
	refreshTouchCycleCounter();
	detectTouchPattern(touchCycleCounterToTouchID());
}

bool Touch::isTouchDetected() {
	return touchDetected;
}

int Touch::getTouchPattern(bool printPattern) {
	//if (printPattern) Serial.printf("Touch pattern: %i \n", activeTouchID);
	return activeTouchID;
}

