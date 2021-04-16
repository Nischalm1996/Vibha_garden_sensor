/* program for dht11 sensor.
1. begin()  initialise variables
2. float readHumidity()
3. float readTemperature()
4. float readTemperatureFah()
5. float heatIndexFah()
6. float heatIndexCel()*/

#ifndef _dht11_H
#define _dht11_H
#include "Arduino.h"
#include "DHT.h"

#define DHTPIN 2 // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

class dht11
{
public:
  void begin()
  {
  }
  float readHumidity()
  {
    return (dht.readHumidity());
  }
  float readTemperature()
  {
    return (dht.readTemperature());
  }
  float readTemperatureFah()
  {
    return (dht.readTemperature(true));
  }
  float heatIndexFah()
  {
    return (dht.computeHeatIndex(readTemperatureFah(),readHumidity()));
  }
  float heatIndexCel()
  {
    return (dht.computeHeatIndex(readTemperature(),readHumidity(),false));
  }
};


#endif //_dht11_H