



//Fonction qui donne une valeur à chacun des 6 bits en fonction des 
void setAddress(int address) {
    // Configurer les broches a1 à a6 en fonction de l'adresse (6 bits)
    digitalWrite(A1_PIN, (address & 0x01));      // Bit 0
    digitalWrite(A2_PIN, (address & 0x02) >> 1); // Bit 1
    digitalWrite(A3_PIN, (address & 0x04) >> 2); // Bit 2
    digitalWrite(A4_PIN, (address & 0x08) >> 3); // Bit 3
    digitalWrite(A5_PIN, (address & 0x10) >> 4); // Bit 4
    digitalWrite(A6_PIN, (address & 0x20) >> 5); // Bit 5
}

digitalWrite(ENABLE_PIN, HIGH); // Activer le multiplexeur
digitalWrite(D_PIN, HIGH);     // Allumer la bobine

digitalWrite(ENABLE_PIN, HIGH); // Activer le multiplexeur
digitalWrite(D_PIN, LOW);      // Éteindre la bobine