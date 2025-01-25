#include"ClassCommunicationSerial.hpp"



unsigned char ClassCommunicationSerial::EncodeMessage(const unsigned char dataToSend,unsigned char Etat) {
    unsigned char messageToSend = 0;
    messageToSend = (Etat << 7) | (dataToSend&0x7F);
    return messageToSend;
}


const std::vector<unsigned char> ClassCommunicationSerial:: EncodeMessageToArray(const unsigned char UserData[]) {
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




int ClassCommunicationSerial::CommunicationSerie(unsigned char* UserInput) {
    if (UserInput[0] != UserInput[1]) {


        // Send and receive data
        for (unsigned char x : EncodeMessageToArray(UserInput)) {
            messageToSend = x;

            // Write to the serial port
            if (!WriteFile(this->hSerial, &messageToSend, sizeof(messageToSend), &this->bytesWritten, NULL)) {
                std::cerr << "Error: Unable to write to the serial port." << std::endl;
                CloseHandle(this->hSerial);
                return 1;
            }

            std::cout << "Message sent (unsigned char):" << std::hex << static_cast<int>(messageToSend) << std::endl;

            // Read from the serial port
            if (!ReadFile(this->hSerial, this->readBuffer, sizeof(this->readBuffer) - 1, &this->bytesRead, NULL)) {
                std::cerr << "Error: Unable to read from the serial port." << std::endl;
                CloseHandle(this->hSerial);
                return 1;
            }

            // Null-terminate the received string
            if (this->bytesRead > 0) {
                this->readBuffer[this->bytesRead] = '\0';
                std::cout << "Response received: " << this->readBuffer << std::endl;
            }

            // Wait 1 second before the next iteration
            Sleep(1000); // Use Sleep (milliseconds) for Windows
        }


    }

    return 0;
}