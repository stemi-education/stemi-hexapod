#ifndef SGBotic_I2CPing_h
#define SGBotic_I2CPing_h

#include "Wire.h"


#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


#define HC_SR04_I2C_ADDR 0x57

class SGBotic_I2CPing {

  public:
    SGBotic_I2CPing();
    
    void start_measure();
    unsigned long get_mesure();
    
    
  private:
    byte SR04_I2CADDR;  
    byte ds[3];
    unsigned long distance = 0;
    int i;
};
#endif