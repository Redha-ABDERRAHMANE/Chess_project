

#define Delais 1000  // Délai en millisecondes
// Pin D2 .. D5 : index de la bobine
// Pin D6 D7 : donnee de selection pour MUX1
// Pin D8: non utilisé
//Pin D9 : prochaine etat de la bobine(activee,desactivee) D
#define GPIO1 D2 
#define GPIO2 D3
#define GPIO3 D4
#define GPIO4 D5
#define GPIO5 D6
#define GPIO6 D7
#define GPIO7 D8 
#define GPIO8 D9 // Pin de D

int GPIO[9] = {GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8};

void setup() {
    // Initialiser la communication série
    Serial.begin(9600);  // Vitesse de communication série (modifiable selon vos besoins)

    // Configurer les broches comme sorties
    for (int i = 0; i < 8; i++) {  // GPIO0 à GPIO7 (D11 à D4)
        pinMode(GPIO[i], OUTPUT);
        digitalWrite(GPIO[i], LOW);  // Assurez-vous que toutes les broches sont initialisées à LOW
    }

    Serial.println("System Initialized");  // Message pour vérifier que le système démarre
}




//Fonction qui donne une valeur à chacun des 6 bits en fonction de l'adresse saisie
//address est un entier représentant une adresse sur 6 bits (0 à 63)
//Chaque bit de address correspond à l’état d’une broche (HIGH ou LOW).

//Objectif : Configurer une adresse sur les multiplexeurs, avec le bit de poids le plus fort représentant l'état D (allumer/éteindre la bobine).
//Elle retourne une adresse de 7 bits.


void activerBroche(unsigned char message) {

    // GPIO[0] et GPIO[1] correspond aux broches de selections du premier MUX 
    // GPIO [2.. 5] correspondent au numero de la bobine
    //GPIO[6] inutilise
    // GPIO[7] utiliser pour indiquer le prochain etat de la bobine (pour activer ou desactiver)

    // Configurer les broches a1 à a6 en fonction de l'adresse (6 bits)
    digitalWrite(GPIO[0], (message & 0x01));      //Envoie le Bit 0 du message à la broche GPIO[0] 
    digitalWrite(GPIO[1], (message & 0x02) >> 1); // Bit 1 à la broche GPIO[1]
    digitalWrite(GPIO[2], (message & 0x04) >> 2); // Bit 2 à la broche GPIO[2]
    digitalWrite(GPIO[3], (message & 0x08) >> 3); // Bit 3 à la broche GPIO[3]
    digitalWrite(GPIO[4], (message & 0x10) >> 4); // Bit 4 à la broche GPIO[4]
    digitalWrite(GPIO[5], (message & 0x20) >> 5); // Bit 5 à la broche GPIO[5]
    digitalWrite(GPIO[7], (message & 0x80) >> 7); // Bit 7 à la broche GPIO[7] pour l'etat de la bobine
    
}


void desactiverBroche() {

    // On evite d'utiliser des boucles pour reduire la complexite du code a executer sur la carte STM32

    // GPIO [0.. 3] correspondent au numero de la bobine
    // GPIO[4] et GPIO[5] correspond aux broches de selections du premier MUX 
    //GPIO[6] inutilisee
    // GPIO[7] utiliser pour indiquer le prochain etat de la bobine (pour activer ou desactiver)

    // Configurer les broches a1 à a6 en fonction de l'adresse (6 bits)

    digitalWrite(GPIO[0], LOW);  //Envoie le Bit 0 du message à la broche GPIO[0] 
    digitalWrite(GPIO[1], LOW); // Bit 1 à la broche GPIO[1]
    digitalWrite(GPIO[2], LOW); // Bit 2 à la broche GPIO[2]
    digitalWrite(GPIO[3], LOW); // Bit 3 à la broche GPIO[3]
    digitalWrite(GPIO[4], LOW); // Bit 4 à la broche GPIO[4]
    digitalWrite(GPIO[5], LOW); // Bit 5 à la broche GPIO[5]
    digitalWrite(GPIO[7], LOW); // Bit 7 à la broche GPIO[7] pour l'etat de la bobine
    
}

void loop() {
    if (Serial.available() > 0) {  // Vérifie s'il y a des données sur l'interface série
        unsigned char receivedData = Serial.read();  // Lire les données comme une chaîne de caractères

        // Extraire le premier caractère comme état
        unsigned char Next_etat = receivedData>>7;

        // Extraire la donnee de selection pour MUX 1
        Serial.print("MUX1: ");
        Serial.println();

        Serial.print("Next_etat: ");
        Serial.write(Next_etat&0x3);

        //Extraire numero de la bobine 

        Serial.print("MUX 2: ");
        Serial.println(Next_etat&0x3C);


        // Attendre un délai avant d'exécuter la commande
        delay(Delais);

        // Appeler la fonction setBobine
        activerBroche(receivedData);

        delay(3000);
        desactiverBroche() ;
    }
}