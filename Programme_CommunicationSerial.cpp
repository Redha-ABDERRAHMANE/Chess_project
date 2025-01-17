#include <windows.h>    // For Windows API functions and structures (e.g., CreateFileA, DCB, COMMTIMEOUTS)
#include <iostream>     // For std::cout, std::cerr, std::endl
#include <string>       // For std::string and std::stoi
#include <cstdio>       // For printf (if you're using printf alongside std::cout)
#include <stdexcept>    // For exception handling in case of std::stoi failure

unsigned char EncodeMessage(const std::string& dataToSend) {
    unsigned char message = 0;

    switch (dataToSend[0]) {
        case 'A':  // Activate the coil
            message = (1 << 7) | (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        case 'D':  // Deactivate the coil
            message = (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        default:
            std::cerr << "Invalid state!" << std::endl;
            break;
    }

    return message;
}






int main(void)
{
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    char readBuffer[256] = {0}; // Buffer de lecture
    unsigned char message ; // Message à envoyer (unsigned char)


    






    // 1. Ouvrir le port COM
    // Le préfixe "\\\\.\\COM1" est nécessaire pour pouvoir ouvrir un port série 
    // sous Windows lorsqu'on utilise CreateFile.
    hSerial = CreateFileA(
        "\\\\.\\COM1",               // Nom du port (à adapter à votre configuration)
        GENERIC_READ | GENERIC_WRITE,// Accès en lecture et écriture
        0,                           // Partage non autorisé
        NULL,                        // Sécurité par défaut
        OPEN_EXISTING,               // On ouvre un port existant
        0,                           // Aucun attribut particulier
        NULL                         // Pas d'utilisation d'un template
    );

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Erreur: Impossible d'ouvrir le port COM.\n");
        return 1;
    }

    // 2. Configurer les paramètres du port (baud rate, bits de données, parité, etc.)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Erreur: GetCommState a échoué.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Exemple de configuration: 9600 bauds, 8N1
    dcbSerialParams.BaudRate = CBR_9600;  // Vitesse: 9600 bauds
    dcbSerialParams.ByteSize = 8;         // 8 bits de données
    dcbSerialParams.StopBits = ONESTOPBIT;// 1 bit de stop
    dcbSerialParams.Parity   = NOPARITY;  // Sans parité

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Erreur: SetCommState a échoué.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 3. Configurer les timeouts (délais de lecture/écriture)
    timeouts.ReadIntervalTimeout         = 50;  // Intervalle max entre deux caractères
    timeouts.ReadTotalTimeoutConstant    = 50;  // Délai constant de lecture
    timeouts.ReadTotalTimeoutMultiplier  = 10;  // Multiplicateur de délai de lecture par octet
    timeouts.WriteTotalTimeoutConstant   = 50;  // Délai constant d'écriture
    timeouts.WriteTotalTimeoutMultiplier = 10;  // Multiplicateur de délai d'écriture par octet

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Erreur: SetCommTimeouts a échoué.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 4. Écriture dans le port COM
    if (!WriteFile(hSerial, &message, sizeof(message), &bytesWritten, NULL))
    {
        printf("Erreur: Impossible d'écrire sur le port COM.\n");
        CloseHandle(hSerial);
        return 1;
    }

    printf("Message envoyé (unsigned char): 0x%X\n", message);

    // 5. Lecture de la réponse
    if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer) - 1, &bytesRead, NULL))
    {
        printf("Erreur: Impossible de lire la réponse.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // On s'assure de terminer la chaîne reçue par un caractère nul
    readBuffer[bytesRead] = '\0';
    printf("Réponse reçue: %s\n", readBuffer);

    // 6. Fermer le port
    CloseHandle(hSerial);

    return 0;
}
