#include "Arduino.h"
#include "DHT11.h"
dhtClass temper;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  temper.updateTemp();
   Serial.print("humidity:");
  Serial.println(humid);
  Serial.print("temperature:");
  Serial.println(temp);
  delay(3000);  
}
