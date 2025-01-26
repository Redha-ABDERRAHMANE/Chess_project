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
