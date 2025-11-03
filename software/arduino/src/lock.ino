#include <Arduino.h>

#define RELAY_PIN 8

const bool RELAY_ACTIVE_LOW = true;

// Durée de l'impulsion pour activer le relais (courte)
const unsigned long UNLOCK_MS = 3000UL;

// Temps avant verrouillage automatique (~1 minute)
const unsigned long AUTO_LOCK_DELAY = 60000UL;

unsigned long unlockStart = 0;
unsigned long lockRequestTime = 0;
bool unlocking = false;
bool lockPending = false;

void initLock() {
    pinMode(RELAY_PIN, OUTPUT);
    if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN, LOW);
    unlocking = false;
    lockPending = false;
    Serial.println("Lock: init, coffre verrouillé");
}

// gestion non bloquante pour ouvrir le coffre
void openBox(bool correctCode) {
    if (correctCode && !unlocking) {
        unlocking = true;
        unlockStart = millis();
        lockPending = true;
        lockRequestTime = millis();

        if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, LOW);
        else digitalWrite(RELAY_PIN, HIGH);

        Serial.println("Lock: coffre déverrouillé");
    }

    // Fin de l'impulsion courte (UNLOCK_MS)
    if (unlocking && millis() - unlockStart >= UNLOCK_MS) {
        unlocking = false; // relais reste actif jusqu'à la minute
    }

    // Vérifie si le coffre doit se verrouiller automatiquement
    if (lockPending && millis() - lockRequestTime >= AUTO_LOCK_DELAY) {
        closeBox();
    }
}

// verrouillage immédiat du coffre
void closeBox() {
    if (RELAY_ACTIVE_LOW) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN, LOW);

    unlocking = false;
    lockPending = false;
    Serial.println("Lock: coffre verrouillé");
}

void lockHandling() {
    static unsigned long unlockTime = 0;
    const unsigned long AUTO_LOCK_DELAY = 60000; // 1 minute

    // si le coffre vient d'être ouvert
    if (unlocking) {
        unlockTime = millis();
        unlocking = false; // on ne relance pas le déverrouillage
    }

    // si le délai est écoulé, verrouiller le coffre
    if ((millis() - unlockTime) >= AUTO_LOCK_DELAY) {
        closeBox(); // verrouillage automatique
        unlockTime = millis(); // évite de rappeler closeBox() à chaque loop
    }
}
