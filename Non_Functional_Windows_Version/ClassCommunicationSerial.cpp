#include"ClassCommunicationSerial.hpp"

// Méthode pour encoder un seul message
unsigned char ClassCommunicationSerial::EncodeMessage(const unsigned char dataToSend, unsigned char Etat) {
    unsigned char messageToSend = 0;
    messageToSend = (Etat << 7) | (dataToSend & 0x7F); // Combine l'état et les données
    return messageToSend;
}

// Méthode pour encoder un tableau de messages en un vecteur
const std::vector<unsigned char> ClassCommunicationSerial::EncodeMessageToArray(const unsigned char UserData[]) {
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

// Méthode pour gérer la communication série
int ClassCommunicationSerial::CommunicationSerie(unsigned char* UserInput) {
    if (UserInput[0] != UserInput[1]) {

        // Envoie et réception des données
        for (unsigned char x : EncodeMessageToArray(UserInput)) {
            messageToSend = x;

            // Écriture dans le port série
            if (!WriteFile(this->hSerial, &messageToSend, sizeof(messageToSend), &this->bytesWritten, NULL)) {
                std::cerr << "Error: Unable to write to the serial port." << std::endl;
                CloseHandle(this->hSerial);
                return 1;
            }

            // Affiche le message envoyé
            std::cout << "Message sent (unsigned char):" << std::hex << static_cast<int>(messageToSend) << std::endl;

            // Attente d'une seconde avant la prochaine itération
            Sleep(100); // Utilise Sleep (en millisecondes) pour Windows
        }
    }

    return 0;
}
