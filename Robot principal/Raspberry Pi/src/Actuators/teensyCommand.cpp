#include "../../include/Robot.h"
#define TEENSY_ADDR 0x08

void Robot::teensy_init(){
    fd_teensy= wiringPiI2CSetup(TEENSY_ADDR);
    if (fd_teensy < 0) {
        std::cerr << "Erreur : Impossible de se connecter à la Teensy !" << std::endl;
    }
}

void Robot::teensy_send_command(uint8_t command){
    if (fd_teensy < 0) return;

    int result = -1;
    int max_retries = 5;
    int attempt = 0;

    while (attempt < max_retries && result < 0) {
        result = wiringPiI2CWrite(fd_teensy, command);
        if (result < 0) {
            std::cerr << "Tentative " << attempt + 1 << " échouée : Impossible d'envoyer la commande à la Teensy !" << std::endl;
            usleep(100000); // Attendre 100 ms avant de réessayer
        }
        attempt++;
    }

    if (result < 0) {
        std::cerr << "Toutes les tentatives ont échoué pour envoyer la commande à la Teensy." << std::endl;
    } else {
        std::cout << "Commande envoyée avec succès après " << attempt << " tentative(s)." << std::endl;
    }
}


