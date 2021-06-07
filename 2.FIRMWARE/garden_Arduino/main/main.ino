
#include "Arduino.h"
#include "dht11.h"
#include "sound.h"
#include "soilMoisture.h"
#include "mpu.h"
#include "Button.h"
#include <Wire.h>
// The I2C LCD library
#include <LiquidMenu.h>

#include <LiquidCrystal_I2C.h>
// The menu wrapper library
// The I2C LCD object


//OK  D5
//LEFT  D7
//RIGHT D4

const bool pullup = false;
Button left(7, pullup);
Button right(4, pullup);
//Button up(8, pullup);
//Button down(9, pullup);
Button enter(5, pullup);


dhtClass temper;
sound Noise;
soilMoisture moisture;
mpuClass angle;

struct allValues {
  float temp_cel;
  float temp_fah;
  float humid;
  int soil_moisture;
  int sound;
  int gyrox;
  int gyroy;
  int gyroz;
};
allValues toEsp = {temp, fahren, 0, 0, 0, 0, 0, 0};
LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidLine welcome_line1(1, 0, "Environment");
LiquidLine welcome_line2(1, 1, "Monitor System");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);
LiquidLine temp1(1, 0, "Temp(Cel):", toEsp.temp_cel);
LiquidLine temp2(1, 1, "Humidity:", toEsp.humid);
LiquidScreen temp_screen(temp1, temp2);
LiquidLine _moisture(1, 0, "Moisture: ", toEsp.soil_moisture);
LiquidLine _Noise(1, 1, "Noise: ", toEsp.sound);
LiquidScreen Screen2(_moisture, _Noise);;
LiquidLine gyrox(1, 0, "Elevation X:", toEsp.gyrox);
LiquidLine gyroy(1, 1, "Elevation Y:", toEsp.gyroy);
LiquidScreen screen3(gyrox, gyroy);

LiquidMenu menu(lcd);

void setup() {
  // put your setup code here, to run once:
 // Serial.begin(9600);
  angle.begin();
  temper.begin();
  lcd.init();
  lcd.backlight();
  delay(500);
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
  // Menu initialization.
  menu.init();
  // This is the method used to add a screen object to the menu.
  menu.add_screen(welcome_screen);
  menu.add_screen(temp_screen);
  menu.add_screen(Screen2);
  menu.add_screen(screen3);
  menu.update();
  delay(500);
  menu.next_screen();
  menu.update();
}
int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  // Check all the buttons
  if (right.check() == LOW)
  {
   // Serial.println(F("RIGHT button pressed"));
    menu.next_screen();
  }
  if (left.check() == LOW)
  {
    //Serial.println(F("LEFT button pressed"));
    menu.previous_screen();
  }
  if (enter.check() == LOW)
  {
    //Serial.println(F("ENTER button pressed"));
    // Switches focus to the next line.
    menu.switch_focus();
  }


  toEsp = {temp, fahren, humid, moisture.getRawReading(), Noise.getNoisePercentage(), xaxis, yaxis, zaxis};
  //  Serial.print("celcius: "); Serial.print(toEsp.temp_cel); Serial.print(' ');
  //  Serial.print("Fahreheit: "); Serial.print(toEsp.temp_fah); Serial.print(' ');
  //  Serial.print("Humid: "); Serial.print(toEsp.humid); Serial.println("");
  //  Serial.print("x: "); Serial.print(toEsp.gyrox); Serial.print(' ');
  //  Serial.print("y: "); Serial.print(toEsp.gyroy); Serial.print(' ');
  //  Serial.print("z: "); Serial.print(toEsp.gyroz); Serial.println("");
  //  Serial.print("soil: "); Serial.println(toEsp.soil_moisture);
  //  Serial.print("sound: "); Serial.println(toEsp.sound);
  //  delay(50);


  count = count + 1;

  if (count >= 30000) {
    temper.updateTemp();
    angle.updateAngles();
    toEsp = {temp, fahren, humid, moisture.getRawReading(), Noise.getNoisePercentage(), xaxis, yaxis, zaxis};

    menu.update();
  }

}
