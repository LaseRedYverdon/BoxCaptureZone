// buttons.ino
// Gestion des boutons : 4 boutons d'équipe avec pression courte et longue (pour reset)

#define REQUIRED_PRESS_SECONDS 5 // durée pour considérer un appui long simultané
extern uint8_t teamCounts[4];  // compteur des captures pour chaque équipe
extern uint8_t captureState;   // état de capture de la zone
extern void commSend(const String& msg); // fonction d'envoi série vers le Raspberry
extern void lcdShowMessage(const char *line1, const char *line2); // affichage LCD

// broches des boutons
const uint8_t BTN_PINS[4] = { 4, 5, 6, 7 }; // R, B, J, V
const uint16_t DEBOUNCE_MS = 30;            // anti-rebond logiciel
const uint16_t LONGPRESS_MS = 5000;         // durée pour appui long individuel (ms)

// tableaux d'état pour le debounce et appuis longs
unsigned long lastChange[4] = { 0,0,0,0 };
bool lastState[4] = { HIGH,HIGH,HIGH,HIGH }; // INPUT_PULLUP -> HIGH au repos
unsigned long pressStart[4] = { 0,0,0,0 };

// initialisation des boutons
void initButtons() {
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(BTN_PINS[i], INPUT_PULLUP);
        lastState[i] = digitalRead(BTN_PINS[i]);
        lastChange[i] = millis();
        pressStart[i] = 0;
    }
    Serial.println("Buttons: initialisation terminée");
}

// fonction appelée lors d'une pression courte sur un bouton
void onButtonShortPress(uint8_t teamIdx) {
    Serial.print("BTN;COURT;"); Serial.println(teamIdx);

    // si le coffre est ouvert, on enregistre la capture
    if (captureState == IDLE) {
        captureState = CAPTURE_PENDING;
        // incrémente le compteur de l'équipe (max 10)
        teamCounts[teamIdx] = (teamCounts[teamIdx] < 10) ? (teamCounts[teamIdx] + 1) : 10;
        commSend("CAPTURE;TEAM;" + String(teamIdx));
        lcdShowMessage("Capturé par l'équipe",
            (teamIdx == 0 ? "ROUGE" : teamIdx == 1 ? "BLEU" : teamIdx == 2 ? "JAUNE" : "VERT"));
    } else {
        // si déjà capturé, logique simplifiée : envoie info pression
        commSend("BTN;PRESS;" + String(teamIdx));
    }
}

// fonction appelée lors d'un appui long simultané sur les 4 boutons
void onButtonLongPress() {
    Serial.println("BTN;LONG;TOUS -> RESET ZONE");
    commSend("RESET;ZONE");

    // remise à zéro des compteurs et état capture
    for (uint8_t i = 0; i < 4; i++) teamCounts[i] = 0;
    captureState = IDLE;
    lcdShowMessage("Zone Réinitialisée", "");
}

// lecture et gestion des boutons (appelée à chaque loop)
void pollButtons() {
    unsigned long now = millis();
    bool allPressed = true;

    for (uint8_t i = 0; i < 4; i++) {
        bool st = digitalRead(BTN_PINS[i]); // HIGH repos, LOW pressé

        // changement d'état détecté -> gestion debounce
        if (st != lastState[i]) {
            if ((now - lastChange[i]) > DEBOUNCE_MS) {
                lastChange[i] = now;
                lastState[i] = st;

                if (st == LOW) {
                    // bouton pressé
                    pressStart[i] = now;
                } else {
                    // bouton relâché
                    unsigned long held = (pressStart[i] ? (now - pressStart[i]) : 0);
                    pressStart[i] = 0;

                    if (held >= LONGPRESS_MS) {
                        // appui long individuel ignoré (seulement 4 boutons simultanés déclenchent reset)
                    } else {
                        // appui court
                        onButtonShortPress(i);
                    }
                }
            }
        }

        // vérifie si tous les boutons sont actuellement pressés
        if (digitalRead(BTN_PINS[i]) != LOW) allPressed = false;
    }

    // vérifie si les 4 boutons sont pressés et maintenus > REQUIRED_PRESS_SECONDS
    static unsigned long allPressedStart = 0;
    if (allPressed) {
        if (allPressedStart == 0) allPressedStart = now;
        if ((now - allPressedStart) >= (REQUIRED_PRESS_SECONDS * 1000UL)) {
            // déclenche le reset de zone
            onButtonLongPress();
            allPressedStart = 0;
            // courte pause pour debounce
            delay(200);
        }
    } else {
        allPressedStart = 0;
    }
}
