#ifndef _mpu_H
#define _mpu_H
#include "Arduino.h"

#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
float xaxis = 0;
float yaxis = 0;
float zaxis = 0;



class mpuClass
{
  public:
    void begin()
    {
      Wire.begin();
      mpu6050.begin();
      mpu6050.calcGyroOffsets(true);

    }
    void updateAngles()
    {
      mpu6050.update();
      xaxis = mpu6050.getAngleX();
      yaxis = mpu6050.getAngleY();
      zaxis = mpu6050.getAngleZ();
    }
};
#endif _mpu_h
