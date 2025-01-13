



//Fonction qui donne une valeur à chacun des 6 bits en fonction de l'adresse saisie
//address est un entier représentant une adresse sur 6 bits (0 à 63)
//Chaque bit de address correspond à l’état d’une broche (HIGH ou LOW).

//Objectif : Configurer une adresse sur les multiplexeurs, avec le bit de poids le plus fort représentant l'état D (allumer/éteindre la bobine).
//Elle retourne une adresse de 7 bits.


void setAddress(unsigned char address, bool d) {

    address &= 0x7F; //on force le bit 7 à 0 puisque unsigned char est codé sur un octet = 8bits; ce bit est inutilisé

    //Ajouter l'état D dans le bit 6
     if (d) {
        address |= 0x40;  // Mettre le bit 6 à 1 si D = 1 (bobine allumée)
    } else {
        address &= ~0x40; // Mettre le bit 6 à 0 si D = 0 (bobine éteinte)
    }


    // Configurer les broches a1 à a6 en fonction de l'adresse (6 bits)
    digitalWrite(A1_PIN, (address & 0x01));      //Envoie le Bit 0 de address à la broche A1_PIN
    digitalWrite(A2_PIN, (address & 0x02) >> 1); // Bit 1 à la broche A2_PIN
    digitalWrite(A3_PIN, (address & 0x04) >> 2); // Bit 2 à la broche A3_PIN
    digitalWrite(A4_PIN, (address & 0x08) >> 3); // Bit 3 à la broche A4_PIN
    digitalWrite(A5_PIN, (address & 0x10) >> 4); // Bit 4 à la broche A5_PIN
    digitalWrite(A6_PIN, (address & 0x20) >> 5); // Bit 5 à la broche A6_PIN

}


//D_PIN est le signal de contrôle qui indique si la bobine doit être alimentée (HIGH) ou coupée (LOW).

void activerBobine() {
    digitalWrite(D_PIN, HIGH);  // Allumer la bobine sélectionnée
}

void desactiverBobine() {
    digitalWrite(D_PIN, LOW);  // Éteindre la bobine sélectionnée
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
