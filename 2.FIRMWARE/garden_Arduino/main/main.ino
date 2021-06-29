#include "Arduino.h"
#include "dht11.h"
#include "sound.h"
#include "soilMoisture.h"
#include "mpu.h"
#include "Button.h"
#include <LiquidMenu.h>
#include <LiquidCrystal_I2C.h>
//push
//OK  D5
//LEFT  D7
//RIGHT D4

const bool pullup = false;

Button left(7, pullup);
Button right(4, pullup);
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
  Serial.begin(9600);

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
  angle.begin();
  temper.begin();
  delay(500);
  menu.next_screen();
  //Serial.println("Setup done");
}
unsigned int count = 0;
void loop() {
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
    menu.switch_focus();
  }
  toEsp = {temp, fahren, humid, moisture.getMoisturePercentage(), Noise.getNoisePercentage(), xaxis, yaxis, zaxis};
  //  Serial.print("celcius: "); Serial.print(toEsp.temp_cel); Serial.print(' ');
  //  Serial.print("Fahreheit: "); Serial.print(toEsp.temp_fah); Serial.print(' ');
  //  Serial.print("Humid: "); Serial.print(toEsp.humid); Serial.println("");
  //  Serial.print("x: "); Serial.print(toEsp.gyrox); Serial.print(' ');
  //  Serial.print("y: "); Serial.print(toEsp.gyroy); Serial.print(' ');
  //  Serial.print("z: "); Serial.print(toEsp.gyroz); Serial.println("");
  //  Serial.print("soil: "); Serial.println(toEsp.soil_moisture);
  //  Serial.print("sound: "); Serial.println(toEsp.sound);
  //  delay(50);

  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '*')
    {
      toEsp = {temp, fahren, humid, moisture.getMoisturePercentage(), Noise.getNoisePercentage(), xaxis, yaxis, zaxis};
      Serial.print("S"); Serial.print(toEsp.temp_cel); Serial.print("A");
      Serial.print(toEsp.humid); Serial.print("B");
      Serial.print(toEsp.soil_moisture); Serial.print("C");
      Serial.print(toEsp.sound); Serial.print("D");
      Serial.print(toEsp.gyrox); Serial.print("E");
      Serial.print(toEsp.gyroy); Serial.println("F"); Serial.print("\n");
    }
  }
  count = count + 1;
  //Serial.print(".");
  if (count >= 1000) {
    temper.updateTemp();
    angle.updateAngles();
    toEsp = {temp, fahren, humid, moisture.getMoisturePercentage(), Noise.getNoisePercentage(), xaxis, yaxis, zaxis};
    //Serial.println("update");
    menu.update();




    count = 0;
  }

}
