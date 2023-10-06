#include <SoftwareSerial.h>
#include "SharedData.h"
#include <Serial.h>

SoftwareSerial softwareSerial;

void setupSerial()
{
    softwareSerial.begin(38400, SWSERIAL_8N1, 2, 13, false);
    if (!softwareSerial)
    {
        Serial.println("Invalid SoftwareSerial pin configuration, check config");
    }
}

String message = "";
int messageSize = -1;
String hash = "";
int messagePartsLength = 0;
String messageParts[100] = {};

void checkSerial()
{
    if (softwareSerial.available() > 0)
    {
        while (softwareSerial.available() > 0)
        {
            char c = softwareSerial.read();
            if (c == '#' && messageSize == -1)
            {
                messageSize = 0;
            }
            else if (c == '#')
            {
                softwareSerial.println("#" + message + "#");
                Serial.println(message);
                createMessageParts();
                createHash();
                handleMessage();
                message = "";
                messageSize = -1;
            }
            else
            {
                messageSize += 1;
                message += c;
            }
        }
    }
}

void createMessageParts()
{
    String part = "";
    messagePartsLength = 0;
    String tmpMessage = message + " ";
    for (int i = 0; i < tmpMessage.length(); i++)
    {
        if (tmpMessage[i] == ' ')
        {
            if (part.length())
            {
                messageParts[messagePartsLength] = part;
                part = "";
                messagePartsLength += 1;
            }
        }
        else
        {
            part += tmpMessage[i];
        }
    }
}

void createHash()
{
    int h = 0;
    // Skip first part
    for (int i = 1; i < messagePartsLength; i++)
    {
        String msg = messageParts[i];
        for (int j = 0; j < msg.length(); j++)
        {
            h += msg[j];
            h %= 256;
        }
    }
    hash = String(h);
}

bool isNumber(String stringNumber)
{
    for (int i = 0; i < stringNumber.length(); i++)
    {
        if (stringNumber[i] >= '0' && stringNumber[i] <= '9')
        {
            return true;
        }
    }
    return false;
}

String keys[] = {"linearVelocity",
                 "direction",
                 "angularVelocity",
                 "translationX",
                 "translationY",
                 "translationZ",
                 "rotationX",
                 "rotationY",
                 "rotationZ",
                 "userSlider",
                 "ledDiretion",
                 "ledSpreadRatio",
                 "ledPrimarClr",
                 "ledSecondarClr",
                 "ledRotationSpeed",
                 "ledBlinkingSpeed",
                 "setLedStatic"};

void handleMessage()
{
    robot.isSerialConnectionOn = true;
    if (messagePartsLength < 3)
    {
        Serial.println("Too few message parts");
        return;
    }
    String messageHash = messageParts[0];
    if (hash != messageHash)
    {
        Serial.println("Hash not valid");
        return;
    }
    for (int i = 1; i < messagePartsLength; i++)
    {
        if (!isNumber(messageParts[i]))
        {
            Serial.println("Param not number");
            return;
        }
    }

    String key = keys[messageParts[1].toInt()];
    if (key == "linearVelocity")
    {
        robot.btInputData.linearVelocity = messageParts[2].toInt();
    }
    else if (key == "direction")
    {
        robot.btInputData.direction = messageParts[2].toInt();
    }
    else if (key == "angularVelocity")
    {
        robot.btInputData.angularVelocity = messageParts[2].toInt();
    }
    else if (key == "translationX")
    {
        robot.btInputData.translationX = messageParts[2].toInt();
    }
    else if (key == "translationY")
    {
        robot.btInputData.translationY = messageParts[2].toInt();
    }
    else if (key == "translationZ")
    {
        robot.btInputData.translationZ = messageParts[2].toInt();
    }
    else if (key == "rotationX")
    {
        robot.btInputData.rotationX = messageParts[2].toInt();
    }
    else if (key == "rotationY")
    {
        robot.btInputData.rotationY = messageParts[2].toInt();
    }
    else if (key == "rotationZ")
    {
        robot.btInputData.rotationZ = messageParts[2].toInt();
    }
    else if (key == "userSlider")
    {
        robot.userSlider = messageParts[2].toInt();
    }
    else if (key == "ledDiretion")
    {
        robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
        robot.btInputData.ledDiretion = messageParts[2].toInt();
    }
    else if (key == "ledSpreadRatio")
    {
        robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
        robot.btInputData.ledSpreadRatio = messageParts[2].toInt();
    }
    else if (key == "ledPrimarClr")
    {
        if (messagePartsLength == 5)
        {
            robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
            robot.btInputData.ledPrimarClr[0] = messageParts[2].toInt();
            robot.btInputData.ledPrimarClr[1] = messageParts[3].toInt();
            robot.btInputData.ledPrimarClr[2] = messageParts[4].toInt();
        }
    }
    else if (key == "ledSecondarClr")
    {
        if (messagePartsLength == 5)
        {
            robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
            robot.btInputData.ledSecondarClr[0] = messageParts[2].toInt();
            robot.btInputData.ledSecondarClr[1] = messageParts[3].toInt();
            robot.btInputData.ledSecondarClr[2] = messageParts[4].toInt();
        }
    }
    else if (key == "ledRotationSpeed")
    {
        robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
        robot.btInputData.ledRotationSpeed = messageParts[2].toInt();
    }
    else if (key == "ledBlinkingSpeed")
    {
        robot.ledCtrl.mode = LED_PARAMETRIC_MODE;
        robot.btInputData.ledBlinkingSpeed = messageParts[2].toInt();
    }
    else if (key == "setLedStatic")
    {
        Color c{static_cast<uint8_t>(messageParts[2].toInt()),
                static_cast<uint8_t>(messageParts[3].toInt()), 
                static_cast<uint8_t>(messageParts[4].toInt())};
        if (messagePartsLength == 5) {
            robot._setLedStatic(c);
        } else if (messagePartsLength == 6) {
            robot._setLedStatic(messageParts[5].toInt(), c);
        }

    }
}
