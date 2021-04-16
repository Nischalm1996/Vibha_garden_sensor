/* program for soil moisture sensor.
1. begin()  initialise variables
2. int getRawReading()
3. float getMoisturePercentage()*/

#ifndef _SOIL_MOISTURE_H
#define _SOIL_MOISTURE_H
#include "Arduino.h"


const int soilSensorPin = A7;
const int lowThreshold = 714;
const int highThreshold = 400;

class soilMoisture
{
  public:
    int getRawReading()
    {
        return(analogRead(soilSensorPin));
    }
    float getMoisturePercentage()
    {
      return(map(getRawReading(),lowThreshold,highThreshold,0,100));
    }
};


#endif //_SOIL_MOISTURE_H