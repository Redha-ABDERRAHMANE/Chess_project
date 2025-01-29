#include"ClassCommunicationSerie.hpp"
#include<bitset>

// Encode un seul message en combinant l'état et les données à envoyer
unsigned char ClassCommunicationSerie::EncodeMessage(const unsigned char dataToSend, unsigned char Etat) {
    return (Etat << 7) | (dataToSend & 0x7F); // Décale l'état sur le bit de poids fort et garde les 7 bits de data
}

// Encode une série de messages sous forme de vecteur
const std::vector<unsigned char> ClassCommunicationSerie::EncodeMessageToArray(const unsigned char UserData[]) {
    std::vector<unsigned char> msgVector; // Initialise un vecteur pour stocker les messages encodés
    int PositionI = UserData[0]; // Position initiale
    int PositionF = UserData[1]; // Position finale
    bool inverse = false; // Indique si les positions doivent être inversées

    // Si les deux positions sont identiques, aucun message à envoyer
    if (UserData[0] == UserData[1]) {
        msgVector.push_back((unsigned char)0);
        return msgVector;
    }

    // Si la position initiale est plus grande que la position finale, inversion
    if (UserData[0] > UserData[1]) {
        PositionI = UserData[1];
        PositionF = UserData[0];
        inverse = true;
    }

    // Boucle pour encoder et stocker les messages dans le vecteur
    for (int i = PositionI; i <= PositionF; i++) {
        msgVector.push_back(EncodeMessage(i, 1)); // Encode chaque valeur et l'ajoute au vecteur
    }

    // Si les valeurs ont été inversées, on inverse l'ordre du vecteur
    if (inverse) {
        std::reverse(msgVector.begin(), msgVector.end());
    }

    return msgVector; // Retourne le vecteur contenant les messages encodés
}


// Fonction de communication série
int ClassCommunicationSerie::CommunicationSerie(unsigned char* UserInput) {

    // Envoi et réception des messages
    for (unsigned char x : EncodeMessageToArray(UserInput)) {
        messageToSend = x; // Stocke le message à envoyer

        // Écrit le message sur le port série
        ssize_t bytesWritten = write(this->serialPort, &messageToSend, sizeof(messageToSend));
        if (bytesWritten < 0) { // Vérifie si l'écriture a échoué
            perror("Erreur : Impossible d'écrire sur le port série");
            close(this->serialPort);
            return 1;
        }

        // Affiche le message envoyé en format binaire
        std::cout << "Message envoyé (unsigned char):" << std::bitset<8>(static_cast<int>(messageToSend))  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause de 100ms entre les envois
    }

    return 0; // Retourne 0 si tout s'est bien passé
}
