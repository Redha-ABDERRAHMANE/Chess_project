#include "ClassCommunicationSerial.hpp"

unsigned char EncodeMessage(const unsigned char dataToSend,unsigned char Etat) {
    unsigned char messageToSend = 0;
    messageToSend = (Etat << 7) | (dataToSend&0x7F);
    return messageToSend;
}


const std::vector<unsigned char> EncodeMessageToArray(const unsigned char UserData[]) {
    std::vector<unsigned char> msgVector;
    int PositionI = UserData[0];
    int PositionF = UserData[1];
    bool inverse = false;

    if(UserData[0]==UserData[1]){
        msgVector.push_back((unsigned char) 0);
        return msgVector;
    }

    if (UserData[0] > UserData[1]) {
        PositionI = UserData[1];
        PositionF = UserData[0];
        inverse = true;
    }

    for (int i = PositionI; i <= PositionF; i++) {
        msgVector.push_back(EncodeMessage(i, 1)); // Add encoded message to the vector
    }

    if (inverse) {
        std::reverse(msgVector.begin(), msgVector.end());
    }

    return msgVector;
}




int CommunicationSerie(unsigned char* UserInput) {
    if (UserInput[0] != UserInput[1]) {
        HANDLE hSerial;
        DWORD bytesWritten, bytesRead;
        char readBuffer[256] = {0};
        unsigned char messageToSend;

        // Open the serial port
        hSerial = CreateFileA(
        "\\\\.\\COM12",               // Nom du port (a adapter a votre configuration)
        GENERIC_READ | GENERIC_WRITE,// Acces en lecture et ecriture
        0,                           // Partage non autorise
        NULL,                        // Securite par defaut
        OPEN_EXISTING,               // On ouvre un port existant
        0,                           // Aucun attribut particulier
        NULL                         // Pas d'utilisation d'un template                    // Null template
        );

        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Error: Unable to open the serial port. Check the COM port." << std::endl;
            return 1;
        }

        // Configure serial port settings
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error: Unable to get serial port state." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        dcbSerialParams.BaudRate = CBR_9600;  // Baud rate: 9600
        dcbSerialParams.ByteSize = 8;        // Data bits: 8
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity   = NOPARITY;

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error: Unable to configure serial port settings." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        // Set timeouts
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 50;         // Max time between two bytes
        timeouts.ReadTotalTimeoutConstant = 50;    // Total timeout for read
        timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplier for read
        timeouts.WriteTotalTimeoutConstant = 50;   // Total timeout for write
        timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplier for write

        if (!SetCommTimeouts(hSerial, &timeouts)) {
            std::cerr << "Error: Unable to configure timeouts." << std::endl;
            CloseHandle(hSerial);
            return 1;
        }

        // Send and receive data
        for (unsigned char x : EncodeMessageToArray(UserInput)) {
            messageToSend = x;

            // Write to the serial port
            if (!WriteFile(hSerial, &messageToSend, sizeof(messageToSend), &bytesWritten, NULL)) {
                std::cerr << "Error: Unable to write to the serial port." << std::endl;
                CloseHandle(hSerial);
                return 1;
            }

            std::cout << "Message sent (unsigned char): 0x" << std::hex << static_cast<int>(messageToSend) << std::endl;

            // Read from the serial port
            if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer) - 1, &bytesRead, NULL)) {
                std::cerr << "Error: Unable to read from the serial port." << std::endl;
                CloseHandle(hSerial);
                return 1;
            }

            // Null-terminate the received string
            if (bytesRead > 0) {
                readBuffer[bytesRead] = '\0';
                std::cout << "Response received: " << readBuffer << std::endl;
            }

            // Wait 1 second before the next iteration
            Sleep(1000); // Use Sleep (milliseconds) for Windows
        }

        // Close the serial port
        CloseHandle(hSerial);
    }

    return 0;
}
