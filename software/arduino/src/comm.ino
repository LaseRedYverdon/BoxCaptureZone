// Comunication with the raspberry
// comm.ino
// Wiegand initialization and serial helper functions

#include <Wiegand.h>
WIEGAND wg;

const uint8_t PIN_DATA0 = 2;
const uint8_t PIN_DATA1 = 3;

const uint16_t PRESET_FACILITY = 0x00; // set your facility
const uint32_t PRESET_CARD = 12345; // set your card id

void initComm() {
    wg.begin(PIN_DATA0, PIN_DATA1);
    Serial.println("Comm: Wiegand started");
}

void commSend(const String &msg) {
    // send to Raspberry Pi via USB serial in a simple parseable format
    Serial.print("COMM;"); Serial.println(msg);
}
