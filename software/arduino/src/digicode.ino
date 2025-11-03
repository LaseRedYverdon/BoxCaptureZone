#include <Wiegand.h>
WIEGAND wg;

String codeEntre = "";
String codeSecret = "1060";
bool codeValide = false;

void initDigicode() {
    wg.begin(2, 3); // D2 = D0, D3 = D1
}

bool readDigicode() {
    //wg.flush();

    if (wg.available()) {
        unsigned long code = wg.getCode();
        byte bits = wg.getWiegandType();
        char caractere = (char)(code & 0xFF);

        if (caractere == '#') {
            codeValide = (codeEntre == codeSecret);
            codeEntre = "";
        } else if (caractere == '*') {
            codeEntre = "";
            codeValide = false; 
        } else if (isDigit(caractere)) {
            codeEntre += caractere;
        }
    }
    
    return codeValide;
}

bool boxIsOpen() {
    return codeValide;
}
