# Box Capture de Zone – LaseRed Yverdon

##  Cahier des charges

- **But du projet** : Coffre électronique avec digicode, boutons lumineux, LEDs RJB, son, et verrouillage par électro-aimant.
    


- **Contraintes** :
    
    - Matériel robuste (usage intensif).
        
    - Maintenance simple.
        
    - Audio de bonne qualité.
        
    - Mise à jour facile des énigmes et sons gérées à distance. 
        
    - Intégration dans un coffre résistant.
        

---


****
##  Architecture générale

### Schéma fonctionnel

```mermaid
graph TD
    A[Clavier numérique] -->|Entrée code| B[Arduino UNO]
    B -->|Contrôle| C[Électro-aimant]
    B -->|Affiche énigme| D[Écran digital]
    B -->|État LEDs| E[10 LEDs RJB]
    B -->|Boutons équipe| F[4 Boutons lumineux]
    G[Raspberry Pi] -->|Énigme / Code| B
    G -->|Audio / Musique| H[Haut-parleur + ampli]
```


```
