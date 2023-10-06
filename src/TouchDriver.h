
#ifndef _TOUCHDRIVER_h
#define _TOUCHDRIVER_h

class Touch {
	private:

		int touch_treshold = 0;
		int cycle_counter_detect_interval = 0;
		int cycle_counter_detect_value = 0;

		unsigned long interruptTime[3] = { 0, 0, 0 };
		int touchCycleCounter[3] = { 0, 0, 0 };

		bool touchDetected = false;
		int activeTouchID = 0;
		int lastTouchID = 0;

		static Touch* instance;
		
		void writeInterruptTime(int touch);
		static void touchInterruptCallbackT4();
		static void touchInterruptCallbackT6();
		static void touchInterruptCallbackT7();

		int touchCycleCounterToTouchID();
		void refreshTouchCycleCounter();
		void detectTouchPattern(int touchID);

	public:
		Touch(int treshold, int interval, int value);

		void checkTouch();
		bool isTouchDetected();
		int getTouchPattern(bool printPattern = false);
		
};

#endif
