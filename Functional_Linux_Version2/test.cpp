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