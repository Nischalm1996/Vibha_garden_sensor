#ifndef _DHT11_H
#define _DHT11_H

#include <dht.h>  // Include library
#define outPin 5  // Defines pin number to which the sensor is connected

dht Dht;      // Creates a DHT object
float temp = 0;
float humid = 0;
float fahren = 0;

class dhtClass
{
  public:
    void updateTemp()
    {
      Dht.read11(outPin);
      temp = Dht.temperature;
       //Serial.println(temp);
      humid = Dht.humidity;
      //Serial.println(humid);
      fahren = ((temp * 9.0) / 5.0 + 32.0);
      //Serial.println(fahren);
    }
};

#endif //_DHT11_H
