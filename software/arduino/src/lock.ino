#include <Arduino.h>
#include "digicode.ino"

#define OUT_LOCK 10

const uint8_t RELAY_PIN = 8;
const bool RELAY_ACTIVE_LOW = true;
const unsigned long UNLOCK_MS = 3000UL;

void initLock() {
    pinMode(RELAY_PIN, OUTPUT);
    if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN, LOW);
    Serial.println("Lock: init");
}

void unlockChest() {
    if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, LOW);
    else digitalWrite(RELAY_PIN, HIGH);
    unsigned long t0 = millis();
    while (millis() - t0 < UNLOCK_MS) {
        // you could monitor sensors here if available
        delay(10);
    }
    if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN, LOW);
}

void initDigicode() {
    pinMode(OUT_LOCK, OUTPUT);
}

void lockHandling() {
    if (boxIsOpen()) {
        digitalWrite(OUT_LOCK, HIGH); // ouvre le coffre
    } else {
        digitalWrite(OUT_LOCK, LOW); // verrouillé
    }
}
