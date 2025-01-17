#define Delais 1000  // Délai en millisecondes
#define GPIO1 D11 
#define GPIO2 D10
#define GPIO3 D9
#define GPIO4 D6
#define NB_BOBINE 4  // Nombre de bobines

int GPIO[5] = {-1, GPIO1, GPIO2, GPIO3, GPIO4};

// Déclaration des fonctions
void setBobine(int Port, char Etat);
void activerBobine(unsigned char message);
void desactiverBobine(unsigned char message);
bool safeAccessGPIO(int index);

void setup() {
    Serial.begin(9600);  // Commencer la connexion série avec 9600 bauds
    for (int i = 1; i <= NB_BOBINE; i++) {
        pinMode(GPIO[i], OUTPUT);  // Configurer les GPIO comme sorties
    }
}

void loop() {
    if (Serial.available() > 0) {  // Vérifie s'il y a des données sur l'interface série
        String receivedData = Serial.readString();  // Lire les données comme une chaîne de caractères

        // Extraire le premier caractère comme état
        char Next_etat = receivedData[0];

        // Extraire le reste comme numéro de port (int)
        int Port = receivedData.substring(1).toInt();
        Serial.print("Port: ");
        Serial.println(Port);

        Serial.print("Next_etat: ");
        Serial.println(Next_etat);

        // Attendre un délai avant d'exécuter la commande
        delay(Delais);

        // Appeler la fonction setBobine
        setBobine(Port, Next_etat);
    }
}

bool safeAccessGPIO(int index) {
    if (index >= 1 && index <= NB_BOBINE) {
        return true;
    } else {
        Serial.println("Invalid GPIO access!");
        return false;
    }
}

void activerBobine(unsigned char message) {
    unsigned char  portIndex= message & 0x3F;  // Extraire le port
    if (safeAccessGPIO(portIndex)) {
        digitalWrite(GPIO[portIndex], HIGH);  // Activer la bobine (HIGH)
    }
}

void desactiverBobine(unsigned char message) {
    unsigned char portIndex = message & 0x3F;  // Extraire le port
    if (safeAccessGPIO(portIndex)) {
        digitalWrite(GPIO[portIndex], LOW);  // Desactiver la bobine (LOW)
    }
}

void setBobine(int Port, char Etat) {
    unsigned char message = 0;  // Déclarer la variable en dehors du switch

    switch (Etat) {
        case 'A':  // Si l'état est 'A', activer la bobine
            message = (1 << 7) | (Port & 0x3F);  // Masquage pour s'assurer que Port est sur 6 bits
            activerBobine(message);
            break;

        case 'D':  // Si l'état est 'D', désactiver la bobine
            message |= (Port & 0x3F);  // Masquage pour s'assurer que Port est sur 6 bits

            desactiverBobine(message);
            break;

        default:
            // Ne rien faire si l'état est invalide
            Serial.println("Invalid state!");
            break;
    }
}
