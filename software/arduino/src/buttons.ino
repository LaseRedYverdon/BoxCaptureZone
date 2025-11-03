// buttons.ino
// Buttons management: 4 team buttons with short press + long press (for reset)

// pins
const uint8_t BTN_PINS[4] = { 4, 5, 6, 7 }; // R, B, J, V
const uint16_t DEBOUNCE_MS = 30;
const uint16_t LONGPRESS_MS = 5000; // 5 seconds to reset

// state arrays
unsigned long lastChange[4] = { 0,0,0,0 };
bool lastState[4] = { HIGH,HIGH,HIGH,HIGH }; // INPUT_PULLUP -> HIGH idle
unsigned long pressStart[4] = { 0,0,0,0 };

void initButtons() {
    for (uint8_t i = 0;i < 4;i++) {
        pinMode(BTN_PINS[i], INPUT_PULLUP);
        lastState[i] = digitalRead(BTN_PINS[i]);
        lastChange[i] = millis();
        pressStart[i] = 0;
    }
    Serial.println("Buttons: init done");
}

void onButtonShortPress(uint8_t teamIdx) {
    // teamIdx 0..3 maps to R,B,J,V
    Serial.print("BTN;SHORT;"); Serial.println(teamIdx);
    // if chest is open we register capture
    // capture logic: first press captures and starts countdown
    if (captureState == IDLE) {
        captureState = CAPTURE_PENDING;
        // increment immediate capture in counts? spec: first click captures immediately
        teamCounts[teamIdx] = (teamCounts[teamIdx] < 10) ? (teamCounts[teamIdx] + 1) : 10;
        commSend("CAPTURE;TEAM;" + String(teamIdx));
        lcdShowMessage("Captured by team", (teamIdx == 0 ? "RED" : teamIdx == 1 ? "BLUE" : teamIdx == 2 ? "YEL" : "GRN"));
    } else {
        // if already captured, may be contest - logic simplified: toggle cancel
        commSend("BTN;PRESS;" + String(teamIdx));
    }
}

void onButtonLongPress() {
    // long press on all 4 resets zone
    Serial.println("BTN;LONG;ALL -> RESET");
    commSend("RESET;ZONE");
    // reset counters and UI
    for (uint8_t i = 0;i < 4;i++) teamCounts[i] = 0;
    captureState = IDLE;
    lcdShowMessage("Zone Reset", "");
}

void pollButtons() {
    unsigned long now = millis();
    bool allPressed = true;
    for (uint8_t i = 0;i < 4;i++) {
        bool st = digitalRead(BTN_PINS[i]); // HIGH idle, LOW pressed
        if (st != lastState[i]) {
            // state changed - debounce
            if ((now - lastChange[i]) > DEBOUNCE_MS) {
                lastChange[i] = now;
                lastState[i] = st;
                if (st == LOW) {
                    // pressed
                    pressStart[i] = now;
                } else {
                    // released
                    unsigned long held = (pressStart[i] ? (now - pressStart[i]) : 0);
                    pressStart[i] = 0;
                    if (held >= LONGPRESS_MS) {
                        // long press by single button - ignore here, require 4 held simultaneously
                        // handled after loop
                    } else {
                        // short press
                        onButtonShortPress(i);
                    }
                }
            }
        }
        // check if currently pressed
        if (digitalRead(BTN_PINS[i]) != LOW) allPressed = false;
    }

    // check if all four pressed and held more than REQUIRED_PRESS_SECONDS
    static unsigned long allPressedStart = 0;
    if (allPressed) {
        if (allPressedStart == 0) allPressedStart = now;
        if ((now - allPressedStart) >= (REQUIRED_PRESS_SECONDS * 1000UL)) {
            // trigger reset
            onButtonLongPress();
            allPressedStart = 0;
            // simple debounce pause
            delay(200);
        }
    } else {
        allPressedStart = 0;
    }
}
