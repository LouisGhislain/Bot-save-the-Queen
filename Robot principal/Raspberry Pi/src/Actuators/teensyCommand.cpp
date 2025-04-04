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

    int result = wiringPiI2CWrite(fd_teensy, command);
    if (result < 0) {
        std::cerr << "Erreur : Impossible d'envoyer la commande à la Teensy !" << std::endl;
    }
}