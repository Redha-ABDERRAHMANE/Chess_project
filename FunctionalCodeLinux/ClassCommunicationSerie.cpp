#include"ClassCommunicationSerie.hpp"
#include<bitset>

// Encode a single message
unsigned char ClassCommunicationSerie::EncodeMessage(const unsigned char dataToSend, unsigned char Etat) {
    return (Etat << 7) | (dataToSend & 0x7F);
}

// Encode messages into a vector
const std::vector<unsigned char> ClassCommunicationSerie::EncodeMessageToArray(const unsigned char UserData[]) {
    std::vector<unsigned char> msgVector;
    int PositionI = UserData[0];
    int PositionF = UserData[1];
    bool inverse = false;

    if (UserData[0] == UserData[1]) {
        msgVector.push_back((unsigned char)0);
        return msgVector;
    }

    if (UserData[0] > UserData[1]) {
        PositionI = UserData[1];
        PositionF = UserData[0];
        inverse = true;
    }

    for (int i = PositionI; i <= PositionF; i++) {
        msgVector.push_back(EncodeMessage(i, 1)); // Add encoded message
    }

    if (inverse) {
        std::reverse(msgVector.begin(), msgVector.end());
    }

    return msgVector;
}


// Serial communication function
int ClassCommunicationSerie::CommunicationSerie(unsigned char* UserInput) {

        // Send and receive messages
        for (unsigned char x : EncodeMessageToArray(UserInput)) {
            messageToSend = x;

            // Write to the serial port
            ssize_t bytesWritten = write(this->serialPort, &messageToSend, sizeof(messageToSend));
            if (bytesWritten < 0) {
                perror("Error: Unable to write to the serial port");
                close(this->serialPort);
                return 1;
            }

            std::cout << "Message sent (unsigned char):" << std::bitset<8>(static_cast<int>(messageToSend))  << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        }
    

    return 0;
}