// leds_internal.ino
// drive 2x 74HC595 cascaded for up to 16 outputs (we use 10)
// using shiftOut (MSB first) - simple and robust
#include <Arduino.h>

const uint8_t SR_DATA = 11;  // DS
const uint8_t SR_CLOCK = 13; // SH_CP
const uint8_t SR_LATCH = 10; // ST_CP

void initShiftRegister() {
    pinMode(SR_DATA, OUTPUT);
    pinMode(SR_CLOCK, OUTPUT);
    pinMode(SR_LATCH, OUTPUT);
    digitalWrite(SR_LATCH, LOW);
    shiftOut(0x00); // clear
    Serial.println("ShiftReg: init done");
}

void shiftOut(uint16_t value16) {
    // send 16 bits MSB first
    digitalWrite(SR_LATCH, LOW);
    for (int i = 15;i >= 0;i--) {
        digitalWrite(SR_CLOCK, LOW);
        digitalWrite(SR_DATA, (value16 >> i) & 1 ? HIGH : LOW);
        digitalWrite(SR_CLOCK, HIGH);
    }
    digitalWrite(SR_LATCH, HIGH);
}

// helper: setBargraph using teamCounts[4] -> stacked 10 LEDs
// layout assumption: LEDs 0..9 correspond to physical positions; mapping is up to PCB.
void setBargraph(uint8_t counts[4]) {
    // Aggregate into a single 16-bit pattern. Strategy: 10 LEDs split as counts per team
    // We'll create zones: first n LEDs show team 0, next show team1, etc.
    // For display, compute counts sum and convert to 10 leds distributed by percentage.
    uint16_t bits = 0;
    uint8_t total = counts[0] + counts[1] + counts[2] + counts[3];
    if (total == 0) {
        // show all white/off pattern (all 0)
        bits = 0x0000;
    } else {
        // compute number of LEDs per team proportional to counts
        uint8_t ledsPerTeam[4];
        uint8_t remaining = 10;
        for (uint8_t i = 0;i < 4;i++) {
            ledsPerTeam[i] = (counts[i] * 10) / total;
            if (ledsPerTeam[i] > remaining) ledsPerTeam[i] = remaining;
            remaining -= ledsPerTeam[i];
        }
        // distribute remaining starting from highest count
        for (uint8_t pass = 0; pass < 4 && remaining>0; pass++) {
            // find index with largest fractional remainder - simple approach: give to highest count
            uint8_t idx = pass; // simple deterministic: R,B,J,V order
            ledsPerTeam[idx] += 1;
            if (ledsPerTeam[idx] > 10) ledsPerTeam[idx] = 10;
            remaining--;
        }
        // build bits: first ledsPerTeam[0] bits set, then next for team1 etc.
        uint8_t pos = 0;
        for (uint8_t t = 0;t < 4;t++) {
            for (uint8_t k = 0;k < ledsPerTeam[t] && pos < 10;k++, pos++) {
                bits |= (1 << pos);
            }
        }
    }
    // Write bits to shift register (lower 10 bits)
    shiftOut(bits);
}
