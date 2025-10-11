#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wiegand.h>


//LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

WIEGAND wg;

void setup() {
    /* lcd.begin(16, 2);
    lcd.print("Hello, world!");
    lcd.setCursor(0, 1);
    lcd.print("LCD OK :)");*/

    pinMode(13, OUTPUT);
}

void loop() {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
}
