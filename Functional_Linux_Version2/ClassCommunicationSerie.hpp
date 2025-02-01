#pragma once

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>

#include <thread>
#include <chrono>

class ClassCommunicationSerie{
private:
        int serialPort = -1; // Descripteur de fichier pour le port série
        struct termios tty={}; // Structure pour la configuration du port série
        unsigned char messageToSend; // Variable pour stocker le message à envoyer
        char readBuffer[256] = {0}; // Tampon pour la lecture des données
public:
    ClassCommunicationSerie(){

        // Ouvre le port série avec accès en lecture et écriture
        this->serialPort = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
        if (this->serialPort < 0) {
            perror("Erreur : Impossible d'ouvrir le port série");
            return ;
        }

        // Récupère la configuration actuelle du port série
        if (tcgetattr(this->serialPort, &tty) != 0) {
            perror("Erreur : Échec de tcgetattr");
            close(this->serialPort);
            return ;
        }

        // Applique la configuration du port série
        if (tcsetattr(this->serialPort, TCSANOW, &tty) != 0) {
            perror("Erreur : Échec de tcsetattr");
            close(this->serialPort);
            return ;
        }

        // Définit la vitesse de communication à 9600 bauds
        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);

        // Configure les paramètres du port série
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // Définit la taille des caractères à 8 bits
        tty.c_cflag |= (CLOCAL | CREAD); // Active la lecture et désactive le contrôle de ligne
        tty.c_cflag &= ~PARENB; // Désactive la parité
        tty.c_cflag &= ~CSTOPB; // Définit un seul bit de stop
        tty.c_cflag &= ~CRTSCTS; // Désactive le contrôle de flux matériel

        // Désactive les options de contrôle de flux logiciel
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_lflag = 0; // Mode brut (désactive le mode canonique)
        tty.c_oflag = 0; // Désactive les options de sortie spéciale

        // Configure les délais de lecture : au moins 1 caractère et attente minimale de 1 décisecondes
        tty.c_cc[VMIN]  = 1;
        tty.c_cc[VTIME] = 1;
    }
    ~ClassCommunicationSerie(){
         if (this->serialPort >= 0) {
            close(this->serialPort); // Ferme le port série si encore ouvert
            }
        }

    // Méthode pour encoder un message
    unsigned char EncodeMessage(const unsigned char dataToSend, unsigned char Etat);
    
    // Méthode pour encoder une série de messages sous forme de tableau
    const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]);
    
    // Méthode pour gérer la communication série
    int CommunicationSerie(unsigned char* UserInput);
};
