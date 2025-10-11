

# Hardware Specification

**Date :** 11/10/2025  
**Version :** 1.0

---

## Vue d’ensemble

- L’Arduino Nano est **alimenté directement par le Raspberry Pi** via le câble USB (5V).
    
- Le Raspberry Pi et l’Arduino partagent la **masse commune (GND)**.
    
- La **communication** entre Nano et Raspberry se fait par **USB série**. Avec le cable d'alimentation du nano
    
- Le **12 V** est utilisé uniquement pour : électro-aimant, relais et luminaires (géré par le raspberry).
    
- Tous les périphériques externes (digikey, boutons, LCD, électro-aimant, relais) doivent être branchés via des **connecteurs amovibles** (borniers ou connecteurs à levier).
    

---

## Schéma simplifié

```
                 +---------------------+
                 |     ARDUINO NANO    |
                 |                     |
        5V  <----| Alimentation depuis Raspberry Pi |
       GND  <----| Masse commune avec Raspberry Pi et périphériques |
                 |                                |
        D0  ---->| Libre / réservé debug          |
        D1  ---->| Libre / réservé debug          |
        D2  ---->| DATA0 Wiegand <-----+---- Clavier métallique Wiegand 26
        D3  ---->| DATA1 Wiegand <-----+---- Clavier métallique Wiegand 26
        D4  ---->| Bouton ROUGE  <-----+---- Bouton poussoir LED 16mm
        D5  ---->| Bouton BLEU   <-----+---- Bouton poussoir LED 16mm
        D6  ---->| Bouton JAUNE  <-----+---- Bouton poussoir LED 16mm
        D7  ---->| Bouton VERT   <-----+---- Bouton poussoir LED 16mm
        D8  ---->| Relais ROUGE  ----> Commande relais (électro-aimant)
        D9  ---->| Relais BLEU   ----> Commande relais (lampes ou autre charge)
       D10  ---->| 74HC595 LATCH ----> Latch pin 74HC595 (pour LEDs RJB)
       D11  ---->| 74HC595 DATA ----> Data pin 74HC595 (pour LEDs RJB)
       D12  ---->| Libre / futur usage audio (géré par Raspberry)
       D13  ---->| 74HC595 CLOCK ---> Clock pin 74HC595 (pour LEDs RJB)
        A4  <--> | I2C SDA  <--------> LCD 1602 I2C (connecteur externe)
        A5  <--> | I2C SCL  <--------> LCD 1602 I2C (connecteur externe)
       A0-A3     | Libre (capteurs/futurs)
       A6-A7     | Libre (capteurs/futurs)
                 +---------------------+

   Entrée 12V : embout rond DC → Relais / électro-aimant / lampes
   Connecteurs externes :
     • Clavier Wiegand (4 fils)
     • Boutons LED (3 fils chacun : signal, +5V, GND)
     • LCD I2C (4 fils)
     • Relais pour électro-aimant et lampes
     • Alimentation 12V
```

---

## Branchements détaillés

**Clavier Wiegand 26**

```
   +12V  ---> VCC clavier
   GND   ---> GND commun
   DATA0 ---> D2 Nano
   DATA1 ---> D3 Nano
   Connecteur 4 broches
```

**Boutons poussoirs LED**

```
   D4/D5/D6/D7 ---> entrée signal
   Autre côté bouton ---> GND
   LED intégrée (si présente) ---> +5V via résistance
   Connecteur 3 broches : Signal / +5V / GND
```

**Shift register 74HC595 (10 LEDs RJB)**

```
   D10 ---> LATCH
   D11 ---> DATA
   D13 ---> CLOCK
   QA–QH ---> LEDs (via résistances) ---> GND
   Si plus de 8 LEDs : cascade second 74HC595
```

**Relais (électro-aimant et lampes)**

```
   D8/D9 ---> IN1/IN2 du module relais
   VCC module ---> 5V Nano
   GND module ---> GND commun
   Sortie relais :
     COM ---> entrée 12V (embout DC)
     NO  ---> électro-aimant ou lampe
   Protection : diode roue libre si DC
```

**LCD 1602 I²C**

```
   SDA ---> A4
   SCL ---> A5
   VCC ---> 5V
   GND ---> GND
   Connecteur 4 broches
```

**Communication avec Raspberry Pi**

```
   USB Nano <--> Raspberry Pi
   Alimentation + communication série
   GND commun
```

**Alimentation générale**

```
   5V Nano <--- Raspberry Pi
   12V ---> relais / électro-aimant / lampes
   GND commun
```


