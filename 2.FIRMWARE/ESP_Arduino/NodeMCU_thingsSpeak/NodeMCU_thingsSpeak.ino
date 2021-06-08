#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
//to read from serial
const byte numChars = 40;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;

// Initialize our values
float temp_cel = 0;
float humid = 0;
int soil_moisture = 0;
int _noise = 0;
int gyrox = 0;
int gyroy = 0;
String myStatus = "";

void setup() {
  Serial.begin(9600);  // Initialize serial
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("Begin");
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print('.');
      delay(1000);
    }
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, temp_cel);
  ThingSpeak.setField(2, humid);
  ThingSpeak.setField(3, soil_moisture);
  ThingSpeak.setField(4, _noise);
  ThingSpeak.setField(5, gyrox);
  ThingSpeak.setField(6, gyroy);

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
  }
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    //Serial.println("Channel update successful.");
  }
  else {
    //Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds to update the channel again
}
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

//============

void parseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index
  strtokIndx = strtok(tempChars, ",");
  temp_cel = atof(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  humid = atof(strtokIndx);

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  soil_moisture = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  _noise = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  gyrox = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  gyroy = atoi(strtokIndx);     // convert this part to an integer
}

void showParsedData() {

  Serial.print("temp_cel ");
  Serial.println(temp_cel);
  Serial.print("humid ");
  Serial.println(humid);
  Serial.print("soil_moisture ");
  Serial.println(soil_moisture );
  Serial.print("_noise ");
  Serial.println(_noise);
  Serial.print("gyrox ");
  Serial.println(gyrox);
  Serial.print("gyroy ");
  Serial.println(gyroy);
}
