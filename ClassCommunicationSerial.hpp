

#include <windows.h> // Include Windows-specific header for serial communication

#include <iostream>
#include <fcntl.h>

#include <unistd.h>
#include <cstring>
#include<vector>
#include <algorithm>

#include <chrono>
#include <thread>



class ClassCommunicationSerial
{
private:
    /* data */

        HANDLE hSerial={0};
        DWORD bytesWritten,bytesRead;
        char readBuffer[256] = {0};
        unsigned char messageToSend;
        DCB dcbSerialParams = {0};
        COMMTIMEOUTS timeouts = {0};

public:
    ClassCommunicationSerial(/* args */){
                // Open the serial port

        this->hSerial = CreateFileA(
        "\\\\.\\COM1",               // Nom du port (a adapter a votre configuration)
        GENERIC_READ | GENERIC_WRITE,// Acces en lecture et ecriture
        0,                           // Partage non autorise
        NULL,                        // Securite par defaut
        OPEN_EXISTING,               // On ouvre un port existant
        0,                           // Aucun attribut particulier
        NULL                         // Pas d'utilisation d'un template                    // Null template
        );

        if (this->hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Error: Unable to open the serial port. Check the COM port." << std::endl;
            
        }
        this->dcbSerialParams.DCBlength = sizeof(this->dcbSerialParams);
        this->dcbSerialParams.BaudRate = CBR_9600;  // Baud rate: 9600
        this->dcbSerialParams.ByteSize = 8;        // Data bits: 8
        this->dcbSerialParams.StopBits = ONESTOPBIT;
        this->dcbSerialParams.Parity   = NOPARITY;

        if (!GetCommState(this->hSerial, &this->dcbSerialParams)) {
            std::cerr << "Error: Unable to get serial port state." << std::endl;
            
           
        }



        if (!SetCommState(this->hSerial, &this->dcbSerialParams)) {
            std::cerr << "Error: Unable to configure serial port settings." << std::endl;
            
           
        }

        // Set timeouts
        this->timeouts.ReadIntervalTimeout = 50;         // Max time between two bytes
        this->timeouts.ReadTotalTimeoutConstant = 50;    // Total timeout for read
        this->timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplier for read
        this->timeouts.WriteTotalTimeoutConstant = 50;   // Total timeout for write
        this->timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplier for write

        if (!SetCommTimeouts(this->hSerial, &this->timeouts)) {
            std::cerr << "Error: Unable to configure this->timeouts." << std::endl;
            
        }

    }
    ~ClassCommunicationSerial(){        // Close the serial port
        CloseHandle(this->hSerial);
    }

    unsigned char EncodeMessage(const unsigned char dataToSend, unsigned char Etat);
    const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]);
    int CommunicationSerie(unsigned char* UserInput);







};