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

void Robot::teensy_separate_stack(){
    std::cout << "Entring in separate stack" << std::endl;
    teensy_send_command(0x02);//RELEASE EXT CANS
    int grab_done = 0;
    do {
        usleep(100000);
        grab_done = wiringPiI2CRead(fd_teensy);
    } while (grab_done != 0x03);
    teensy_send_command(0x04); //SEND CREMAILLERE
}
