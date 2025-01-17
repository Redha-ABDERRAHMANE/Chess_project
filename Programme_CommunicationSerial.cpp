#include <windows.h>    // For Windows API functions and structures (e.g., CreateFileA, DCB, COMMTIMEOUTS)
#include <iostream>     // For std::cout, std::cerr, std::endl
#include <string>       // For std::string and std::stoi
#include <cstdio>       // For printf (if you're using printf alongside std::cout)
#include <stdexcept>    // For exception handling in case of std::stoi failure





std::string UserInput(){
    std::string  message;
    std::cout<<"Veuillez ecrire une position a faire bouger le pion"<<std::endl; 
    std::cin>> message;
    return message;
    
}
unsigned char EncodeMessage(const std::string& dataToSend) {
    unsigned char messageToSend = 0;

    switch (dataToSend[0]) {
        case 'A':  // Activate the coil
            messageToSend = (1 << 7) | (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        case 'D':  // Deactivate the coil
            messageToSend = (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        default:
            std::cerr << "Invalid state!" << std::endl;
            break;
    }

    return messageToSend;
}






int main(void)
{
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    char readBuffer[256] = {0}; // Buffer de lecture
    std::string messageUser;
    unsigned char messageToSend ; // messageToSend a envoyer (unsigned char)









    // 1. Ouvrir le port COM
    // Le prefixe "\\\\.\\COM1" est necessaire pour pouvoir ouvrir un port serie 
    // sous Windows lorsqu'on utilise CreateFile.
    hSerial = CreateFileA(
        "\\\\.\\COM5",               // Nom du port (a adapter a votre configuration)
        GENERIC_READ | GENERIC_WRITE,// Acces en lecture et ecriture
        0,                           // Partage non autorise
        NULL,                        // Securite par defaut
        OPEN_EXISTING,               // On ouvre un port existant
        0,                           // Aucun attribut particulier
        NULL                         // Pas d'utilisation d'un template
    );




    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Erreur: Impossible d'ouvrir le port COM.\n");
        return 1;
    }

    // 2. Configurer les parametres du port (baud rate, bits de donnees, parite, etc.)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Erreur: GetCommState a echoue.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Exemple de configuration: 9600 bauds, 8N1
    dcbSerialParams.BaudRate = CBR_9600;  // Vitesse: 9600 bauds
    dcbSerialParams.ByteSize = 8;         // 8 bits de donnees
    dcbSerialParams.StopBits = ONESTOPBIT;// 1 bit de stop
    dcbSerialParams.Parity   = NOPARITY;  // Sans parite

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Erreur: SetCommState a echoue.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 3. Configurer les timeouts (delais de lecture/ecriture)
    timeouts.ReadIntervalTimeout         = 50;  // Intervalle max entre deux caracteres
    timeouts.ReadTotalTimeoutConstant    = 50;  // Delai constant de lecture
    timeouts.ReadTotalTimeoutMultiplier  = 10;  // Multiplicateur de delai de lecture par octet
    timeouts.WriteTotalTimeoutConstant   = 50;  // Delai constant d'ecriture
    timeouts.WriteTotalTimeoutMultiplier = 10;  // Multiplicateur de delai d'ecriture par octet

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Erreur: SetCommTimeouts a echoue.\n");
        CloseHandle(hSerial);
        return 1;
    }
while(messageUser!="C"){

    messageUser=UserInput();
    messageToSend=EncodeMessage(messageUser);

    // 4. ecriture dans le port COM
    if (!WriteFile(hSerial, &messageToSend, sizeof(messageToSend), &bytesWritten, NULL))
    {
        printf("Erreur: Impossible d'ecrire sur le port COM.\n");
        CloseHandle(hSerial);
        return 1;
    }

    printf("messageToSend envoye (unsigned char): 0x%X\n", messageToSend);

    // 5. Lecture de la reponse
    if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer) - 1, &bytesRead, NULL))
    {
        printf("Erreur: Impossible de lire la reponse.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // On s'assure de terminer la chaîne reçue par un caractere nul
    readBuffer[bytesRead] = '\0';
    printf("Reponse reçue: %s\n", readBuffer);



}

    // 6. Fermer le port
    CloseHandle(hSerial);

    return 0;
}
