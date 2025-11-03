#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // adresse I2C et dimensions

void initLCD() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    Serial.println("LCD: init");
}

void lcdShowMessage(const char *line1, const char *line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
