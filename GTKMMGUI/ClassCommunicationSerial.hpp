

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

    hSerial = CreateFileA(
        "\\\\.\\COM12",                  // Name of the COM port
        GENERIC_READ | GENERIC_WRITE,    // Access mode: read and write
        0,                               // Share mode: none
        NULL,                            // Security attributes: default
        OPEN_EXISTING,                   // Open an existing COM port
        0,                               // No special file attributes
        NULL                             // No template file
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        DWORD errorCode = GetLastError();
        std::cerr << "Error: Unable to open the serial port. Error code: " << errorCode << std::endl;

        // Optional: You can handle specific errors here
        if (errorCode == ERROR_FILE_NOT_FOUND) {
            std::cerr << "The specified COM port does not exist. Ensure the port name is correct." << std::endl;
        } else if (errorCode == ERROR_ACCESS_DENIED) {
            std::cerr << "Access denied. The COM port may be in use by another application." << std::endl;
        }

        // Exit or handle the error gracefully
        return;
        
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