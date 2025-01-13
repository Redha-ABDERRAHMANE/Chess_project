#define Delais 1000  // Délai en millisecondes
#define GPIO1 D11 
#define GPIO2 D10
#define GPIO3 D9
#define GPIO4 D6

int GPIO[5] = {-1,GPIO1, GPIO2, GPIO3, GPIO4};

// Déclaration des fonctions
void setBobine(int Port, char Etat);
void activerBobine(int Port);
void desactiverBobine(int Port);

void setup() {
    Serial.begin(9600);  // Commencer la connexion série avec 9600 bauds
    for (int i = 1; i < 5; i++) {
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

void activerBobine(int Port) {
    digitalWrite(GPIO[Port], HIGH);  // Activer la bobine (HIGH)
}

void desactiverBobine(int Port) {
    digitalWrite(GPIO[Port], LOW);  // Désactiver la bobine (LOW)
}

void setBobine(int Port, char Etat) {
    
    switch (Etat) {
        unsigned char address=0 ;   //une variable adresse où le bit de poid fort est le bit enable (HIGH/LOW) et le reste des bits sont les bits pour designer le port
        case 'A':  // Si l'état est 'A', activer la bobine
            address=(1<<7 )|Port;
            activerBobine(address &3F);
            break;
        case 'D':  // Si l'état est 'D', désactiver la bobine
            address|=Port;
            
            desactiverBobine(address &3F);
            break;
        default:
            break;  // Ne rien faire si l'état est invalide
    }
}
