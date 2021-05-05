#ifndef _DISPLAY_MENU_1602_H
#define _DISPLAY_MENU_1602_H
#include "Arduino.h"
#include <Wire.h>
// The I2C LCD library
#include <LiquidCrystal_I2C.h>
// The menu wrapper library
#include <LiquidMenu.h>
// The I2C LCD object
#include "Button.h"

#include "dht11.h"
#include "soilMoisture.h"


dht11 dhtobj;
soilMoisture soilobj;


//OK  D5
//LEFT  D4
//RIGHT D7
//BACK  D6
//unsigned int mq2 = 0;
//unsigned int mq3 = 0;
//unsigned int mq7 = 0;
//unsigned int mq135 = 0;
// Button objects instantiation
const bool pullup = false;
Button left(8, pullup);
Button right(7, pullup);
//Button up(8, pullup);
//Button down(9, pullup);
Button enter(6, pullup);

LiquidCrystal_I2C lcd(0x27, 16, 2);

LiquidLine welcome_line1(1, 0, "environment monitor");
LiquidLine welcome_line2(1, 1, "system");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);
LiquidLine gasLine1(1, 0, "dht11", "soilMoisture");
LiquidLine gasLine2(1, 1, dht11, soilMoisture);
LiquidScreen GASSCREEN1(gasLine1, gasLine2);
LiquidLine gasLine3(1, 0, "MQ7      ", "MQ135");
LiquidLine gasLine4(1, 1, mq7, mq135);
LiquidScreen GASSCREEN2(gasLine3, gasLine4);
LiquidLine clickLine(1, 0, " Click OK to");
LiquidLine spray_line(5, 1, " SPRAY");
LiquidLine demo_line(5, 1, " DEMO");
LiquidScreen screen3(clickLine, spray_line);
LiquidScreen screen4(clickLine, demo_line);

LiquidMenu menu(lcd);

void sprayliquid()
{
  Serial.println(F("Spray"));
  sprayObj.motor(50);
  ledObj.Blue(1000);
}
void showDemo()
{
  //send message with values
Serial.println(F("Demo"));
  gsm.sendMessage("LEVEL EXCEEDED!", "+919060344544");

  //turn led red
  ledObj.Red(1000);
}

class displayMenu1602
{

  public:
    void begin()
    {

      Serial.begin(9600);
      sprayObj.begin();
      ledObj.begin();
      ledObj.Red(500);
      ledObj.Blue(500);
      ledObj.Green(500);

      // This is the I2C LCD object initialization.
      //lcd.begin(16, 2);
      lcd.init();
      lcd.backlight();
      delay(500);
      lcd.noBacklight();
      delay(500);
      lcd.backlight();
      demo_line.attach_function(1, showDemo);
      spray_line.attach_function(1, sprayliquid);
      // Menu initialization.
      menu.init();
      // This is the method used to add a screen object to the menu.
      menu.add_screen(welcome_screen);
      menu.add_screen(GASSCREEN1);
      menu.add_screen(GASSCREEN2);
      menu.add_screen(screen3);
      menu.add_screen(screen4);
      menu.update();
      delay(1000);
      menu.next_screen();

    }

    void runMenu()
    {
      // Check all the buttons
      if (right.check() == LOW)
      {
        Serial.println(F("RIGHT button pressed"));
        menu.next_screen();
      }
      if (left.check() == LOW)
      {
        Serial.println(F("LEFT button pressed"));
        menu.previous_screen();
      }
      if (enter.check() == LOW)
      {
        Serial.println(F("ENTER button pressed"));
        // Switches focus to the next line.
        menu.switch_focus();
        //get the spray system running
        menu.call_function(1);
      }

      mq2 = mq2obj.getMq2Value();
      mq3 = mq3obj.getMq3Value();
      mq7 = mq7obj.getMq7Value();
      mq135 = mq135obj.getMq135Value();
    }
};

#endif //_DISPLAY_MENU_1602_H