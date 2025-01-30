#include"ClassCommunicationSerie.hpp"
// Encode un seul message en combinant l'état et les données à envoyer
unsigned char ClassCommunicationSerie::EncodeMessage(const unsigned char dataToSend, unsigned char Etat) {
    return (Etat << 7) | (dataToSend & 0x7F); // Décale l'état sur le bit de poids fort et garde les 7 bits de data
}




std::vector<int> FindIndex(const unsigned char target, const std::vector<std::vector<int>>& matrix) {
    int rowIndex = 0, colIndex = 0;

    for (int row = 0; row < matrix.size(); ++row) {
        auto iteration = std::find(matrix[row].begin(), matrix[row].end(), target); // chercher la position de la bobine dans la matrice 3x3
        
        if (iteration != matrix[row].end()) { // Si on n'arrive pas à la fin du vecteur alors nous avons trouvé la colonne et la ligne de la bobine
            colIndex = std::distance(matrix[row].begin(), iteration); // Calculer la colonne en faisant la difference entre l'iterateur au debut et l'iterateur it
            rowIndex = row;
            break; //On arrete la boucle for dès qu'on trouve la colonne et la ligne liées à la position de la bobine
        }
    }
    return {rowIndex, colIndex};
}

const std::vector<unsigned char> ClassCommunicationSerie::EncodeMessageToArray(const unsigned char UserData[]) {
    std::vector<unsigned char> msgVector; // Initialise un vecteur pour stocker les messages encodés

    // Matrice représentant la grille avec les valeurs des bobines.
    // Les cases contenant '0' sont des emplacements vides (inaccessibles).
    std::vector<std::vector<int>> matrix = {
        {1,  2,  3,  4,  5,  6,  7},
        {8,  0,  0,  9,  0,  0,  10},
        {11, 0,  0,  12, 0,  0,  13},       // ATTENTION: Cette matrice peut varier, cela depend de votre preference
        {14, 15, 16, 17, 18, 19, 20},       //Mais il faut changer le calcul de position dans la fonction position dans Grille.cpp
        {21, 0,  0,  22, 0,  0,  23},
        {24, 0,  0,  25, 0,  0,  26},
        {27, 28, 29, 30, 31, 32, 33}
    };

    // Trouver les coordonnées (ligne, colonne) des positions initiale et finale dans la matrice
    std::vector<int> PositionI = FindIndex(UserData[0], matrix);
    std::vector<int> PositionF = FindIndex(UserData[1], matrix);

    // Si les deux positions sont identiques ou invalides (non trouvées), aucun message à envoyer
    if (PositionF == PositionI) {
        msgVector.push_back((unsigned char)0);
        return msgVector;
    }

    bool inverse = false;
    
    // Si la position initiale est plus basse dans la matrice que la finale, on les échange pour toujours aller de haut en bas
    if (PositionI[0] > PositionF[0]) {   
        std::swap(PositionI, PositionF);  
        inverse = true; // On note qu'on a inversé l'ordre
    }

    std::vector<int> chemin; // Vecteur pour stocker le chemin des bobines à allumer

    // Cas où les deux positions sont sur la même ligne (mouvement horizontal)
    if (PositionI[0] == PositionF[0]) {
        int row = PositionI[0]; // Ligne commune
        for (int col = std::min(PositionI[1], PositionF[1]); col <= std::max(PositionI[1], PositionF[1]); ++col) {
            if (matrix[row][col] != 0) { // On ignore les cases vides (0)
                chemin.push_back(matrix[row][col]); // Ajouter la position au chemin
            }
        }
    }
    // Cas où les deux positions sont sur la même colonne (mouvement vertical)
    else if (PositionI[1] == PositionF[1]) {
        int col = PositionI[1]; // Colonne commune
        for (int row = PositionI[0]; row <= PositionF[0]; ++row) {
            if (matrix[row][col] != 0) { // Ignorer les cases vides
                chemin.push_back(matrix[row][col]); // Ajouter la position au chemin
            }
        }
    }
    // Cas où le mouvement est diagonal ou mixte (chemin combiné vertical et horizontal)
    else {
        int row = PositionI[0];
        int col = PositionI[1];

        // Boucle jusqu'à ce qu'on atteigne la position finale
        while (row != PositionF[0] || col != PositionF[1]) {
            if (matrix[row][col] != 0) { // Vérifier que la position est valide
                chemin.push_back(matrix[row][col]); // Ajouter au chemin
            }

            // Ajustement de la ligne (déplacement vertical)
            if (row < PositionF[0]) row++;
            else if (row > PositionF[0]) row--;

            // Ajustement de la colonne (déplacement horizontal)
            if (col < PositionF[1]) col++;
            else if (col > PositionF[1]) col--;
        }

        // Ajouter la dernière position au chemin si elle est valide
        if (matrix[row][col] != 0) {
            chemin.push_back(matrix[row][col]);
        }
    }

    // Encoder chaque position du chemin en message
    for (int pos : chemin) { // Parcourir le chemin des bobines à allumer dans le vecteur chemin
        msgVector.push_back(EncodeMessage(pos, 1)); // Encoder chaque position
    }

    // Si l'ordre a été inversé au début, on réinverse le chemin avant de l'envoyer
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
