/*
  This is a library for the HC-SR04 I2C Ultrasonic sensor.
  This version of HC-SR04 uses I2C for communication. Two pins (SCL / SDA)
  are required to interface to the device. This library does not work with
  HC-SR04 with GPIO interface.
  Written by KC, SGBotic.com 2020.
*/

#include "SGBotic_I2CPing.h"

// Define milliseconds delay for ESP8266 platform
#if defined(ESP8266)

#include <pgmspace.h>
#define _delay_ms(ms) delayMicroseconds((ms)*1000)

// Use _delay_ms from utils for AVR-based platforms
#elif defined(__avr__)
#include <util/delay.h>

// Use Wiring's delay for compability with another platforms
#else
#define _delay_ms(ms) delay(ms)
#endif

SGBotic_I2CPing::SGBotic_I2CPing()
{
  SR04_I2CADDR = HC_SR04_I2C_ADDR;
  ds[0] = 0;
  ds[1] = 0;
  ds[2] = 0;
}

void SGBotic_I2CPing::start_measure()
{
  Wire.beginTransmission(SR04_I2CADDR);
  Wire.write(1);
  Wire.endTransmission();
}

unsigned long SGBotic_I2CPing::get_mesure()
{
  ds[0] = 0;
  ds[1] = 0;
  ds[2] = 0;
  i = 0;
  byte retBytes = Wire.requestFrom(SR04_I2CADDR, 3);
  if (retBytes == 0) {
    return 9999;
  }
  while (Wire.available())
  {
    ds[i++] = Wire.read();
  }

  distance = (unsigned long)(ds[0]) * 65536;
  distance = distance + (unsigned long)(ds[1]) * 256;
  distance = (distance + (unsigned long)(ds[2])) / 10000;

  return distance;
}