#define Delais 1000  // Délai en millisecondes
#define GPIO1 D11 
#define GPIO2 D10
#define GPIO3 D9
#define GPIO4 D6
#define NB_BOBINE 4  // Nombre de bobines

int GPIO[5] = {-1, GPIO1, GPIO2, GPIO3, GPIO4};

// Déclaration des fonctions
void setBobine(int Port, char Etat);
void activerBobine(unsigned char address);
void desactiverBobine(unsigned char address);
bool safeAccessGPIO(int index);

void setup() {
    Serial.begin(9600);  // Commencer la connexion série avec 9600 bauds
    for (int i = 1; i <= NB_BOBINE; i++) {
        pinMode(GPIO[i], OUTPUT);  // Configurer les GPIO comme sorties
    }
}

void loop() {
    int port;
    char Etat;

    for(int i=1, i<GPIO.size();i++){

        // Appeler la fonction setBobine
        setBobine(GPIO[i], 'A');

 
        Serial.print("Port: ");
        Serial.println(Port);

        Serial.print("Next_etat: ");
        Serial.println('A');

        // Attendre un délai avant d'exécuter la commande
        delay(Delais);
        // Appeler la fonction setBobine
        setBobine(GPIO[i], 'D');

 
        Serial.print("Port: ");
        Serial.println(Port);

        Serial.print("Next_etat: ");
        Serial.println('D');

        // Attendre un délai avant d'exécuter la commande
        delay(Delais);

 
    
}

bool safeAccessGPIO(int index) {
    if (index >= 1 && index <= NB_BOBINE) {
        return true;
    } else {
        Serial.println("Invalid GPIO access!");
        return false;
    }
}

void activerBobine(unsigned char address) {
    unsigned char  portIndex= address & 0x3F;  // Extraire le port
    if (safeAccessGPIO(portIndex)) {
        digitalWrite(GPIO[portIndex], HIGH);  // Activer la bobine (HIGH)
    }
}

void desactiverBobine(unsigned char address) {
    unsigned char portIndex = address & 0x3F;  // Extraire le port
    if (safeAccessGPIO(portIndex)) {
        digitalWrite(GPIO[portIndex], LOW);  // Desactiver la bobine (LOW)
    }
}

void setBobine(int Port, char Etat) {
    unsigned char address = 0;  // Déclarer la variable en dehors du switch

    switch (Etat) {
        case 'A':  // Si l'état est 'A', activer la bobine
            address = (1 << 7) | (Port & 0x3F);  // Masquage pour s'assurer que Port est sur 6 bits
            activerBobine(address);
            break;

        case 'D':  // Si l'état est 'D', désactiver la bobine
            address |= (Port & 0x3F);  // Masquage pour s'assurer que Port est sur 6 bits

            desactiverBobine(address);
            break;

        default:
            // Ne rien faire si l'état est invalide
            Serial.println("Invalid state!");
            break;
    }
}
