/* program for soil moisture sensor.
1. begin()  initialise variables
2. int getRawReading()
3. float getMoisturePercentage()*/

#ifndef _SOUND_H
#define _SOUND_H
#include "Arduino.h"


const int soundSensorPin = A3;
const int lowNoiseThreshold = 0;
const int highNoiseThreshold = 1023;

class sound
{
  public:
    int getReading()
    {
        return(analogRead(soundSensorPin));
    }
    float getNoisePercentage()
    {
      //return(map(getReading(),lowNoiseThreshold,highNoiseThreshold,0,100));
      return(getReading());
    }
};

#endif //_SOUND_H
