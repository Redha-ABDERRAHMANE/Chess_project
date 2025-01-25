
#include <windows.h> // Inclut l'en-tête spécifique à Windows pour la communication série
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

// Fonction pour encoder un seul message
unsigned char EncodeMessage(const unsigned char dataToSend, unsigned char Etat) {
    unsigned char messageToSend = 0;
    messageToSend = (Etat << 7) | (dataToSend & 0x7F); // Combine l'état et les données
    return messageToSend;
}

// Fonction pour encoder un tableau de messages en un vecteur
const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]) {
    std::vector<unsigned char> msgVector;
    int PositionI = UserData[0];
    int PositionF = UserData[1];
    bool inverse = false;

    // Si les deux positions sont identiques, ajoute un seul élément (0)
    if (UserData[0] == UserData[1]) {
        msgVector.push_back((unsigned char)0);
        return msgVector;
    }

    // Si la position initiale est supérieure à la position finale, inverse les positions
    if (UserData[0] > UserData[1]) {
        PositionI = UserData[1];
        PositionF = UserData[0];
        inverse = true;
    }

    // Ajoute les messages encodés au vecteur
    for (int i = PositionI; i <= PositionF; i++) {
        msgVector.push_back(EncodeMessage(i, 1)); // Ajoute un message encodé
    }

    // Inverse le vecteur si nécessaire
    if (inverse) {
        std::reverse(msgVector.begin(), msgVector.end());
    }

    return msgVector;
}

// Fonction pour gérer la communication série
int CommunicationSerie(unsigned char* UserInput) {
    if (UserInput[0] != UserInput[1]) {
        HANDLE hSerial; // Handle pour le port série
        DWORD bytesWritten, bytesRead; // Variables pour les opérations d'écriture et de lecture
        char readBuffer[256] = {0}; // Tampon pour lire les données reçues
        unsigned char messageToSend;

        // Ouvre le port série
        hSerial = CreateFileA(
            "\\\\.\\COM2",               // Nom du port (à adapter à votre configuration)
            GENERIC_READ | GENERIC_WRITE, // Accès en lecture et écriture
            0,                           // Pas de partage
            NULL,                        // Sécurité par défaut
            OPEN_EXISTING,               // Ouvre un port existant
            0,                           // Pas d'attribut spécial
            NULL                         // Pas de modèle
        );

        // Vérifie si le port série a été ouvert avec succès
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Error: Unable to open the serial port. Check the COM port." << std::endl;
            return 1;
        }

        // Configure les paramètres du port série
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        // Vérifie si les paramètres du port série peuvent être obtenus
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error: Unable to get serial port state." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        // Configure les paramètres du port série (baud rate, bits de données, etc.)
        dcbSerialParams.BaudRate = CBR_9600;  // Vitesse en bauds : 9600
        dcbSerialParams.ByteSize = 8;        // Nombre de bits : 8
        dcbSerialParams.StopBits = ONESTOPBIT; // 1 bit d'arrêt
        dcbSerialParams.Parity   = NOPARITY;   // Pas de parité

        // Applique les paramètres configurés
        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error: Unable to configure serial port settings." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        // Configure les délais d'attente
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 50;         // Délai maximum entre deux octets
        timeouts.ReadTotalTimeoutConstant = 50;    // Délai total pour la lecture
        timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplicateur pour la lecture
        timeouts.WriteTotalTimeoutConstant = 50;   // Délai total pour l'écriture
        timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplicateur pour l'écriture

        // Applique les délais d'attente
        if (!SetCommTimeouts(hSerial, &timeouts)) {
            std::cerr << "Error: Unable to configure timeouts." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        // Boucle pour envoyer et recevoir des données
        for (unsigned char x : EncodeMessageToArray(UserInput)) {
            messageToSend = x;

            // Écrit dans le port série
            if (!WriteFile(hSerial, &messageToSend, sizeof(messageToSend), &bytesWritten, NULL)) {
                std::cerr << "Error: Unable to write to the serial port." << std::endl;
                CloseHandle(hSerial);
                return 1;
            }

            // Affiche le message envoyé
            std::cout << "Message sent (unsigned char): 0x" << std::hex << static_cast<int>(messageToSend) << std::endl;

            // Lit les données reçues du port série
            if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer) - 1, &bytesRead, NULL)) {
                std::cerr << "Error: Unable to read from the serial port." << std::endl;
                CloseHandle(hSerial);
                return 1;
            }

            // Termine la chaîne lue avec un caractère nul
            if (bytesRead > 0) {
                readBuffer[bytesRead] = '\0';
                std::cout << "Response received: " << readBuffer << std::endl;
            }

            // Attente d'une seconde avant la prochaine itération
            Sleep(1000); // Utilise Sleep (en millisecondes) pour Windows
        }

        // Ferme le port série
        CloseHandle(hSerial);
    }

    return 0;
}

int main() {
    unsigned char userI[2];
    userI[0] = (unsigned char)1; // Position initiale
    userI[1] = (unsigned char)2; // Position finale

    CommunicationSerie(userI); // Appelle la fonction de communication série

    return 0;
}
