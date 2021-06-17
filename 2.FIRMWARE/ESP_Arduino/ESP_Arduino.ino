#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


String myStatus = "";

byte flag;

char c;
String dataIn;
int8_t indexofA, indexofB, indexofC, indexofD, indexofE, indexofS, indexofF, a, b;
String temp_cel, humid, soil_moisture, _noise,
       gyrox, gyroy;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

}
unsigned long time_now;
void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(3000);
    }
    Serial.println("\nConnected.");
    time_now  = millis();

  }

  delay(10);
  if (millis() < (time_now + 20000)) {
    flag = 0;
  }
  else {
    flag = 1;
    //Serial.print(".");
  }
  if (flag == 1)
  {
    //flushy();

    readSerialData();

    sendToThingsSpeak();
    flag = 0;
    time_now = millis();
  }



}
//////////////////////////////////////////////////////////////////////////////////
void parse_the_data()
{
  indexofS = dataIn.indexOf("S");
  indexofA = dataIn.indexOf("A");
  indexofB = dataIn.indexOf("B");
  indexofC = dataIn.indexOf("C");
  indexofD = dataIn.indexOf("D");
  indexofE = dataIn.indexOf("E");
  indexofF = dataIn.indexOf("F");

  temp_cel = dataIn.substring (indexofS + 1 , indexofA);
  humid = dataIn.substring (indexofA + 1, indexofB);
  soil_moisture = dataIn.substring (indexofB + 1, indexofC);
  _noise = dataIn.substring (indexofC + 1, indexofD);
  gyrox = dataIn.substring (indexofD + 1, indexofE);
  gyroy = dataIn.substring (indexofE + 1, indexofF);
}

void readSerialData()
{
  flushy();
  Serial.println('*');
  delay(100);
  while (Serial.available() > 0)
  {
    c = Serial.read();
    if (c == '\n') {
      break;
    }
    else    {
      dataIn = dataIn + c;
    }
    //Serial.println(c);

  }
  Serial.println(dataIn);
  if (c == '\n')
  {
    parse_the_data();
  }
  c = 0;
  dataIn = "";
//  Serial.println("temp_cel=" + temp_cel);
//  Serial.println("humidity=" + humid);
//  Serial.println("Soil Moisture=" + soil_moisture);
//  Serial.println("Noise=" + _noise);
//  Serial.println("Gyro x axis=" + gyrox);
//  Serial.println("Gyro y axis=" + gyroy);
}

void sendToThingsSpeak()
{
  Serial.println("temp_cel=" + temp_cel);
  Serial.println("humidity=" + humid);
  Serial.println("Soil Moisture=" + soil_moisture);
  Serial.println("Noise=" + _noise);
  Serial.println("Gyro x axis=" + gyrox);
  Serial.println("Gyro y axis=" + gyroy);
  ThingSpeak.setField(1, temp_cel);
  ThingSpeak.setField(2, humid);
  ThingSpeak.setField(3, soil_moisture);
  ThingSpeak.setField(4, _noise);
  ThingSpeak.setField(5, gyrox);
  ThingSpeak.setField(6, gyroy);
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
void flushy()
{
  while (Serial.available())
    Serial.read();
}
