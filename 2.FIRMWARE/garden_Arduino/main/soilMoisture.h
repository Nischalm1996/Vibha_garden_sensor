/* program for soil moisture sensor.
1. begin()  initialise variables
2. int getRawReading()
3. float getMoisturePercentage()*/

#ifndef _SOIL_MOISTURE_H
#define _SOIL_MOISTURE_H
#include "Arduino.h"


const int soilSensorPin = A7;
const int lowThreshold = 0;
const int highThreshold = 1023;

class soilMoisture
{
  public:
    int getRawReading()
    {
        return(analogRead(soilSensorPin));
    }
    int getMoisturePercentage()
    {
      return(map(getRawReading(),lowThreshold,highThreshold,100,0));
    }
};


#endif //_SOIL_MOISTURE_H
