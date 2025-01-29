#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include<vector>
#include <algorithm>

#include <chrono>
#include <thread>


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



int CommunicationSerie(unsigned char* UserInput){

    if(UserInput[0]!=UserInput[1]){

        int serialPort;
        struct termios tty;
        char readBuffer[256] = {0};
        std::string messageUser;
        unsigned char messageToSend;

        // 1. Open the serial port
        serialPort = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
        if (serialPort < 0) {
            perror("Error: Unable to open the serial port");
            return 1;
        }

        // 2. Configure serial port settings
        if (tcgetattr(serialPort, &tty) != 0) {
            perror("Error: tcgetattr failed");
            close(serialPort);
            return 1;
        }

        // Set baud rate to 9600
        cfsetospeed(&tty, B9600); // Set output baud rate to 9600
        cfsetispeed(&tty, B9600); // Set input baud rate to 9600

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 data bits
        tty.c_cflag |= (CLOCAL | CREAD);           // Enable receiver and set local mode
        tty.c_cflag &= ~PARENB;                    // No parity
        tty.c_cflag &= ~CSTOPB;                    // 1 stop bit
        tty.c_cflag &= ~CRTSCTS;                   // No hardware flow control

        tty.c_iflag &= ~(IXON | IXOFF | IXANY);    // Disable software flow control
        tty.c_lflag = 0;                           // Non-canonical mode
        tty.c_oflag = 0;                           // Raw output

        tty.c_cc[VMIN]  = 1;                       // Read at least 1 character
        tty.c_cc[VTIME] = 1;                       // Timeout in deciseconds

        if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
            perror("Error: tcsetattr failed");
            close(serialPort);
            return 1;
        }

        for(unsigned char x: EncodeMessageToArray(UserInput) ){

            messageToSend = x;

            // 3. Write to the serial port
            ssize_t bytesWritten = write(serialPort, &messageToSend, sizeof(messageToSend));
            if (bytesWritten < 0) {
                perror("Error: Unable to write to the serial port");
                close(serialPort);
                return 1;
            }

            std::cout << "Message sent (unsigned char): 0x" << std::hex << static_cast<int>(messageToSend) << std::endl;

            // 4. Read from the serial port
            ssize_t bytesRead = read(serialPort, readBuffer, sizeof(readBuffer) - 1);
            if (bytesRead < 0) {
                perror("Error: Unable to read from the serial port");
                close(serialPort);
                return 1;
            }

            // Null-terminate the received string
            readBuffer[bytesRead] = '\0';
            std::cout << "Response received: " << readBuffer << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));


        }
            // 5. Close the serial port
            close(serialPort);

    }

    return 0;

}












int main() {


    unsigned char userI[2];
    userI[0]= (unsigned char ) 1;
    userI[1]= (unsigned char ) 2;


    CommunicationSerie(userI);
 
}
