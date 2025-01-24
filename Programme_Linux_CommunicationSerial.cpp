#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

std::string UserInput() {
    std::string input;
    std::cout << "Enter message (C to exit): ";
    std::getline(std::cin, input);
    return input;
}



unsigned char EncodeMessage(const std::string& dataToSend) {
    unsigned char messageToSend = 0;
    

    switch (dataToSend[0]) {
        case 'A':  // Activate the coil
            messageToSend = (1 << 7) | (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        case 'D':  // Deactivate the coil
            messageToSend = (std::stoi(dataToSend.substr(1)) & 0x7F);
            break;

        default:
            std::cerr << "Invalid state!" << std::endl;
            break;
    }

    return messageToSend;
}



int main() {
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

    while (messageUser != "C") {
        messageUser = UserInput();
        if (messageUser == "C") break;

        messageToSend = EncodeMessage(messageUser);

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
    }

    // 5. Close the serial port
    close(serialPort);

    return 0;
}
