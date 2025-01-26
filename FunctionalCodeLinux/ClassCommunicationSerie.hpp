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
        int serialPort = -1;
        struct termios tty={};
        unsigned char messageToSend;
        char readBuffer[256] = {0};
public:
    ClassCommunicationSerie(){

        // Open the serial port
        this->serialPort = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
        if (this->serialPort < 0) {
            perror("Error: Unable to open the serial port");
            return ;
        }

        // Configure serial port settings
        if (tcgetattr(this->serialPort, &tty) != 0) {
            perror("Error: tcgetattr failed");
            close(this->serialPort);
            return ;
        }



        if (tcsetattr(this->serialPort, TCSANOW, &tty) != 0) {
            perror("Error: tcsetattr failed");
            close(this->serialPort);
            return ;
        }



        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_lflag = 0;
        tty.c_oflag = 0;

        tty.c_cc[VMIN]  = 1;
        tty.c_cc[VTIME] = 1;
    }
    ~ClassCommunicationSerie(){
         if (this->serialPort >= 0) {
            close(this->serialPort);
            }
        }

    unsigned char EncodeMessage(const unsigned char dataToSend, unsigned char Etat);
    const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]);
    int CommunicationSerie(unsigned char* UserInput);

};

