

#define Delais 1000  // Délai en millisecondes
#define GPIO1 D11 
#define GPIO2 D10
#define GPIO3 D9
#define GPIO4 D6

int GPIO[9] = {GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8};


//Fonction qui donne une valeur à chacun des 6 bits en fonction de l'adresse saisie
//address est un entier représentant une adresse sur 6 bits (0 à 63)
//Chaque bit de address correspond à l’état d’une broche (HIGH ou LOW).

//Objectif : Configurer une adresse sur les multiplexeurs, avec le bit de poids le plus fort représentant l'état D (allumer/éteindre la bobine).
//Elle retourne une adresse de 7 bits.


void setAddress(unsigned char address, bool d) {

    if (d) {
        address |= 0x80;  // Mettre le bit 7 à 1 si D = 1 (bobine allumée)
    } else {
        address &= 0x7F;  // Mettre le bit 7 à 0 si D = 0 (bobine éteinte)
    }


    // Configurer les broches a1 à a6 en fonction de l'adresse (6 bits)
    digitalWrite(GPIO[0], (address & 0x01));      //Envoie le Bit 0 de address à la broche A1_PIN
    digitalWrite(GPIO[1], (address & 0x02) >> 1); // Bit 1 à la broche A2_PIN
    digitalWrite(GPIO[2], (address & 0x04) >> 2); // Bit 2 à la broche A3_PIN
    digitalWrite(GPIO[3], (address & 0x08) >> 3); // Bit 3 à la broche A4_PIN
    digitalWrite(GPIO[4], (address & 0x10) >> 4); // Bit 4 à la broche A5_PIN
    digitalWrite(GPIO[5], (address & 0x20) >> 5); // Bit 5 à la broche A6_PIN

}


//D_PIN est le signal de contrôle qui indique si la bobine doit être alimentée (HIGH) ou coupée (LOW).

void activerBobine() {
    digitalWrite(GPIO[6], HIGH);  // Allumer la bobine sélectionnée
}

void desactiverBobine() {
    digitalWrite(GPIO[6], LOW);  // Éteindre la bobine sélectionnée
}





void setBobine(int address, char Etat) {
    switch (Etat) {
        case 'A':  // Activer
            setAddress(address, true);  // Allumer la bobine
            activerBobine();
            break;
        case 'D':  // Désactiver
            setAddress(address, false); // Éteindre la bobine
            desactiverBobine();
            break;
        default:
            Serial.println("Commande invalide !");
            break;
    }
}
