# STEMI Hexapod v2 API documentation #

Library that includes everything you need to get your STEMI hexapod walking and dancing using your mobile app :)

## LED ##

#### Functions for animated led colors. Combine these functions to get LED animations: ####

```c++
void setLed(Color color);
```
* Examples:
```c++
robot.setLed(CYAN);
```
* Arguments:
    * Color

---

```c++
void setLed(Color primarClr, Color secondarClr, uint8_t spreadRatio, int16_t direction);
```
* Exaples:
```c++
robot.setLed(PURPLE, YELLOW, 50, 30);  
robot.setLed(PURPLE, YELLOW, 50, LED_L1_ANGLE);
```
* Arguments:
    * Color,
    * Color,
    * [0,100] %,
    * [-180, 180] °

---

```c++
void setLedRotationSpeed(int8_t rotationSpeed);
```
* Example:
```c++
robot.setLedRotationSpeed(50);
```
* Units:
    * [-100,100] %

---

```c++
void setLedBlinkingSpeed(uint8_t blinkingSpeed);
```
* Example:
```c++
robot.setLedBlinkingSpeed(50);
```
* Arguments:
    * [0,100] %

---

#### Functions for static led colors - without animations (rotation and blinking don’t work when combined with these functions): ####

```c++
void setLedStatic(uint8_t ledNo, Color color);
```
* Examples:
```c++
robot.setLedStatic(4, RED);
robot.setLedStatic(LED_L2, BLUE);
```
* Arguments:
    * [0,5] led number,
    * Color

---

```c++
void setLedStatic(Color color);
```
* Examples:
```c++
robot.setLedStatic(RED);
```
* Arguments:
    * Color

---

#### Possible inputs for: ``` Color ``` ####
```c++
RED = { 255, 0, 0 };
GREEN = { 0, 255, 0 };
BLUE = { 0, 0, 255 };
YELLOW = { 255, 242, 0 };
PURPLE = { 255, 0, 255 };
CYAN = { 0, 255, 255 };
WHITE = { 255, 255, 255 };
ORANGE = { 255, 60, 0 };
BLACK = { 0, 0, 0 };
```

#### Possible inputs for: ``` ledNo ``` ####
```c++
LED_R1 = 0
LED_R2 = 1
LED_R3 = 2
LED_L1 = 5
LED_L2 = 4
LED_L3 = 3
```

#### Possible inputs for: ``` direction ``` ####
```c++
LED_R1_ANGLE  = -30
LED_R2_ANGLE = -90
LED_R3_ANGLE = -120
LED_L1_ANGLE = 30
LED_L2_ANGLE = 90
LED_L3_ANGLE = 120
```

# Movement #

```c++
void move(userPresetInputData movement, float duration = MOVE_DURATION_DEFAULT);
```
* Examples:
```c++
robot.move(FORWARD, 500);
```
* Arguments:
    * movement,
    * milliseconds (Note: duration can be specified but is not obligatory, default is infinite = -1)

---

```c++
void move(uint8_t linearVelocity, int16_t direction, int8_t angularVelocity, float duration = MOVE_DURATION_DEFAULT);
```
* Examples:
```c++
robot.move(40, 0, 0); //move forward infinitely, or until other command is called
robot.move(0, 0, 40); //rotate to the left infinitely, or until other command is called
robot.move(40, 0, 0, 5000) //move forward for 5000 miliseconds
```
* Arguments:
    * [0,100] %,
    * [-180,180] °,
    * [-100,100] %,
    * milliseconds (Note: duration can be specified but is not obligatory, default is infinite = -1)

---

```c++
void rotate(userPresetInputData rotation, float duration = MOVE_DURATION_DEFAULT);
```
* Examples:
```c++
robot.rotate(LEFT, 5000);
```
* Arguments:
    * movement,
    * milliseconds (Note: duration can be specified but is not obligatory, default is infinite = -1)

---

```c++
void tilt(userPresetInputData tiltation);
```
* Examples:
```c++
robot.tilt(LEFT);
```
* Arguments:
    * tiltation

---

```c++
void tilt(int8_t rotationXNew, int8_t rotationYNew, int8_t rotationZNew);
```
* Examples:
```c++
robot.tilt(30, 0 , 50);
```
* Arguments:
    * [-100,100] %,
    * [-100,100] %,
    * [-100,100] %
    Note: check robot axes

---

```c++
void stretch(userPresetInputData stretchment);
```
* Examples:
```c++
robot.stretch(LEFT);
```
* Arguments:
    * stretchment

---

```c++
void stretch(int8_t translationXNew, int8_t treanslationYNew);
```
* Examples:
```c++
robot.stretch(30, 0);
```
* Arguments:
    * [-100,100] %,
    * [-100,100] %
    Note: check robot axes

---

```c++
void setHeight(uint8_t height);
```
* Examples:
```c++
robot.setHeight(30);
```
* Arguments:
    * [0,100] %

---

#### Possible inputs for: ``` userPresetInputData (movement, tiltation, stretchment) ``` ####
```
FORWARD
BACKWARD
RIGHT
LEFT
RESET
```

---

## Modes ##


```c++
int8_t getMode(); //Returns ID of current mode
void enterUserMode(); //Enters user mode
void exitUserMode(); //Enters walking mode
```

#### Possible modes: ####

```
ROBOT_USER_MODE -1
ROBOT_STANDBY_MODE 0
ROBOT_WALK_MODE 1
ROBOT_DANCE_MODE 2
ROBOT_SETUP_MODE 10
ROBOT_CALIBRATION_MODE 11
ROBOT_BATTERY_EMPTY_MODE 12
```

---

## Touch sensors ##

```c++
uint8_t getTouchPattern(); //Returns touch pattern if pressed, else it returns 0 or TOUCH_000.
```
* Examples:
```c++
int touchPattern = robot.getTouchPattern();
```

#### Possible ```touchPattern``` values: ####
```
TOUCH_000 0
TOUCH_X00 1
TOUCH_0X0 2
TOUCH_XX0 3
TOUCH_00X 4
TOUCH_X0X 5
TOUCH_0XX 6
TOUCH_XXX 7
```

---

## Battery ##
```c++
float getBatteryVoltage(); //returns current battery voltage
uint8_t getBatteryPercentage(); // returns current battery percentage
```

---

## Data ##
All user input data is stored in ```userInputData```, an object made out of the following structure:
```c++
struct InputData
{
	// Movement - inherited from MovementData
    uint8_t linearVelocity; // [0,100]%
	int16_t direction; // [-180,180]degree
	int8_t  angularVelocity; // [-100,100]%

	// Pose - inherited from PoseData
	int8_t translationX; // [-100,100]%
	int8_t translationY; // [-100,100]%
	int8_t translationZ; // [-100,100]%
	int8_t rotationX; // [-100,100]%
	int8_t rotationY; // [-100,100]%
	int8_t rotationZ; // [-100,100]%
	int8_t poseSpeed; // [0,100]% - temporary [99,30]

    // Led - inherited from LedData
	uint8_t ledPrimarClr[3]; // [255]r, [255]g, [255]b
	uint8_t ledSecondarClr[3]; // [255]r, [255]g, [255]b
	int16_t ledDiretion; // [-180,180]degree
	uint8_t ledSpreadRatio; // [0,100]%
	int8_t ledRotationSpeed; // [-100,100]%
	uint8_t ledBlinkingSpeed; // [0,100]%
	uint8_t ledManualClr[6][3]; // 6x [255]r, [255]g, [255]b
	int8_t ledMode; // [manual, parametric]mode

	// Misc - additional data
	// int8_t robotMode; //[check ROBOT_XX_MODE macros]mode
	int8_t moveDuration = 0; // seconds of current command execution [-2 = already written, -1 = inf, 0 = go home, 1-100 = seconds to move]
	int8_t gaitID = 3; //[0,5]gait
	uint8_t stepHeight = 50; // [0,100]%
	bool servoPower = 1; // [on,off]power
};
```
Example for accesing:
```c++
int8_t poseSpeedstate = robot.userInputData.poseSpeed;
int8_t linearVelocityRead = robot.userInputData.linearVelocity;
```
