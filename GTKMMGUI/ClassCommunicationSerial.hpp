#include <windows.h> // Inclut l'en-tête spécifique à Windows pour la communication série
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

// Déclaration de la classe pour gérer la communication série
class ClassCommunicationSerial
{
private:
    // Données membres privées
    HANDLE hSerial = {0}; // Handle pour le port série
    DWORD bytesWritten, bytesRead; // Variables pour les écritures et lectures sur le port série
    char readBuffer[256] = {0}; // Tampon pour stocker les données reçues
    unsigned char messageToSend; // Message à envoyer
    DCB dcbSerialParams = {0}; // Structure pour les paramètres de communication série
    COMMTIMEOUTS timeouts = {0}; // Structure pour les délais d'attente

public:
    // Constructeur de la classe
    ClassCommunicationSerial() {
        // Ouvre le port série
        hSerial = CreateFileA(
            "\\\\.\\COM2",               // Nom du port (doit correspondre à votre configuration)
            GENERIC_READ | GENERIC_WRITE, // Accès en lecture et écriture
            0,                           // Pas de partage
            NULL,                        // Sécurité par défaut
            OPEN_EXISTING,               // Ouvre un port existant
            0,                           // Aucun attribut particulier
            NULL                         // Pas d'utilisation de modèle
        );

        // Vérifie si le port série a été ouvert avec succès
        if (hSerial == INVALID_HANDLE_VALUE) {
            DWORD errorCode = GetLastError(); // Récupère le code d'erreur
            std::cerr << "Error: Unable to open the serial port. Error code: " << errorCode << std::endl;

            // Gestion des erreurs spécifiques
            if (errorCode == ERROR_FILE_NOT_FOUND) {
                std::cerr << "The specified COM port does not exist. Ensure the port name is correct." << std::endl;
            } else if (errorCode == ERROR_ACCESS_DENIED) {
                std::cerr << "Access denied. The COM port may be in use by another application." << std::endl;
            }

            // Sortie ou gestion de l'erreur
            return;
        }

        // Initialise les paramètres de communication série
        this->dcbSerialParams.DCBlength = sizeof(this->dcbSerialParams);
        this->dcbSerialParams.BaudRate = CBR_9600;  // Vitesse en bauds : 9600
        this->dcbSerialParams.ByteSize = 8;         // Nombre de bits : 8
        this->dcbSerialParams.StopBits = ONESTOPBIT; // 1 bit d'arrêt
        this->dcbSerialParams.Parity   = NOPARITY;   // Pas de parité

        // Vérifie si les paramètres du port série peuvent être obtenus
        if (!GetCommState(this->hSerial, &this->dcbSerialParams)) {
            std::cerr << "Error: Unable to get serial port state." << std::endl;
        }

        // Applique les paramètres de communication série
        if (!SetCommState(this->hSerial, &this->dcbSerialParams)) {
            std::cerr << "Error: Unable to configure serial port settings." << std::endl;
        }

        // Configure les délais d'attente
        this->timeouts.ReadIntervalTimeout = 50;         // Temps maximum entre deux octets
        this->timeouts.ReadTotalTimeoutConstant = 50;    // Temps total d'attente pour la lecture
        this->timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplicateur pour la lecture
        this->timeouts.WriteTotalTimeoutConstant = 50;   // Temps total d'attente pour l'écriture
        this->timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplicateur pour l'écriture

        // Applique les délais d'attente
        if (!SetCommTimeouts(this->hSerial, &this->timeouts)) {
            std::cerr << "Error: Unable to configure this->timeouts." << std::endl;
        }
    }

    // Destructeur de la classe
    ~ClassCommunicationSerial() {
        CloseHandle(this->hSerial); // Ferme le port série
    }

    // Méthode pour encoder un message unique
    unsigned char EncodeMessage(const unsigned char dataToSend, unsigned char Etat);

    // Méthode pour encoder un tableau de messages en un vecteur
    const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]);

    // Méthode pour gérer la communication série
    int CommunicationSerie(unsigned char* UserInput);
};
