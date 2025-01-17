#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>



unsigned char CoderMessage(std::string dataTosend){
        unsigned char message = 0;  // Déclarer la variable en dehors du switch

    switch (dataTosend[0]) {
        case 'A':  // Si l'état est 'A', activer la bobine


            message = (1 << 7) | (std::stoi(dataTosend.substr(1))  & 0x7F);  // Masquage pour s'assurer que Port est sur 6 bits
    
            break;

        case 'D':  // Si l'état est 'D', désactiver la bobine
            message=(std::stoi(dataTosend.substr(1))  & 0x7F);  // Masquage pour s'assurer que Port est sur 6 bit
            break;

        default:
            // Ne rien faire si l'état est invalide
            std::cout<<("Invalid state!")<<std::endl;
            break;
    }
}
int main() {
    std::string portName = "/dev/ttyCOM5"; // Replace with your port name (e.g., COM3 for Windows)
    std::ofstream serialPort;

    // Open the serial port
    serialPort.open(portName, std::ios::out | std::ios::binary);
    if (!serialPort.is_open()) {
        std::cerr << "Failed to open serial port: " << portName << std::endl;
        return 1;
    }

    std::cout << "Serial port " << portName << " opened successfully!" << std::endl;

    while (true) {
        std::string dataToSend;
        std::cout << "Enter data to send: ";
        std::getline(std::cin, dataToSend);

        // Send data to Arduino
        serialPort << dataToSend << "\n";
        serialPort.flush(); // Ensure data is sent immediately
        std::cout << "Data sent: " << dataToSend << std::endl;

        // Optional: Add a delay between messages
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Close the serial port
    serialPort.close();
    return 0;
}
