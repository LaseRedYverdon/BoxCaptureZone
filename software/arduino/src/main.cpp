/*
  ============================================================================
  Projet : Box Capture de Zone - LASERED Yverdon
  Système : Arduino Nano (unité de contrôle)
  Objet   : Gestion du digicode, LEDs, boutons, et électro-aimant
  Auteur  : Mirco Profico
  ============================================================================

   Schéma de connexion (Arduino Nano)

  ┌─────────────────────────────┬────────────┬────────┬────────────────────────────┐
  │ Élément                    │ Broche     │ Type   │ Description                │
  ├─────────────────────────────┼────────────┼────────┼────────────────────────────┤
  │ Clavier numérique 4x4      │ D2–D5      │ Entrée │ Lignes du digicode         │
  │                            │ D6–D9      │ Entrée │ Colonnes du digicode       │
  │ Écran LCD (I2C)            │ A4 (SDA)   │ Sortie │ Communication I2C          │
  │                            │ A5 (SCL)   │ Sortie │                            │
  │ Électro-aimant (MOSFET)    │ D10        │ Sortie │ Verrouillage du coffre     │
  │ Bouton Rouge               │ D11        │ Entrée │ Équipe rouge               │
  │ Bouton Bleu                │ D12        │ Entrée │ Équipe bleue               │
  │ Bouton Jaune               │ D13        │ Entrée │ Équipe jaune               │
  │ Bouton Vert                │ A0         │ Entrée │ Équipe verte               │
  │ Registre à décalage 74HC595│ D14 (DATA) │ Sortie │ Données LEDs internes      │
  │                            │ D15 (LATCH)│ Sortie │ Verrouillage du registre   │
  │                            │ D16 (CLOCK)│ Sortie │ Horloge de décalage        │
  │ Communication série (Pi)   │ D0 (RX)    │ Série  │ Réception depuis Pi        │
  │                            │ D1 (TX)    │ Série  │ Envoi vers Pi              │
  │                            │ A6         │ Sortie │                            │
  │ Bouton Reset global        │ A7         │ Entrée │ Réinitialisation système   │
  └─────────────────────────────┴────────────┴────────┴────────────────────────────┘

 Notes :
  - Clavier 4x4 géré en balayage matriciel (8 broches)
  - Bus I2C partagé uniquement par l’écran
  - Les LEDs internes peuvent être reliées via registre à décalage (74HC595)
  - Communication série bidirectionnelle avec Raspberry Pi
  - Tous les boutons utilisent résistances pull-down internes ou externes

  ============================================================================
*/

#include <Wiegand.h>
#include <Wire.h>

#include "digicode.ino"       // lecture code à 4 chiffres
#include "lock.ino"           // contrôle électro-aimant
#include "buttons.ino"        // boutons équipe
#include "leds.ino"  // LEDs RJB via 74HC595
#include "comm.ino"           // communication série avec Pi
#include "screen.ino"

void setup() {
    Serial.begin(9600);

    while (!Serial) {}

    commSend("SYSTEM;READY");
    lcdShowMessage("SYSTEM;READY", "");

    initDigicode();      // configure les broches D2/D3 et interne
    initLock();          // configure broche électro-aimant
    initButtons();       // configure les 4 boutons couleur
    initShiftRegister(); // configure registre à décalage pour LEDs
    initComm();          // initialise communication série avec Pi
    initLCD();           // initialise l'écran lcd

    lcdShowMessage("Enigme : 5050 + 10", "");
}

void loop() {
    // 1) Lecture du digicode
    if (readDigicode()) { // renvoie true si code correct
        unlockChest();     // ouvre le coffre
        lcdShowMessage("Access Granted", "Open box & press");
        commSend("ACCESS;GRANTED");
    }

    // 2) Vérifie si verrou doit se refermer automatiquement
    lockHandling(); // ferme si délai écoulé

    // 3) Lecture et traitement des boutons couleur
    pollButtons();  // détecte appui simple ou long
    //updateTeamCapture(); // met à jour les temps de capture par équipe

    // 4) Met à jour LEDs bargraph selon capture équipe
   // setBargraph(teamCounts);

    // 5) Envoi des infos au Raspberry Pi
    //sendTeamStatus(); // messages type "TEAM;R;3" ou "TEAM;B;5"

    delay(10); // léger délai pour ne pas saturer le Nano
}
