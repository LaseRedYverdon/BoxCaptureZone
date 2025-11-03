// Comunication with the raspberry
// comm.ino
// Wiegand initialization and serial helper functions


const uint8_t PIN_DATA0 = 2;
const uint8_t PIN_DATA1 = 3;
 

void initComm() {

}

void commSend(const String &msg) {
    // send to Raspberry Pi via USB serial in a simple parseable format
    Serial.print("COMM;"); Serial.println(msg);
}
