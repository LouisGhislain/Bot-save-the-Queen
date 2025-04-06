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
    /*
    0. vérifier sa position avec les infrarouges
    1. activer 4 aimants
    2. lever de quelques millimètres
    3. descendre les doigts */

    //teensy_send_command(0x01);
    /*teensy_send_command(0x02);
    int grab_done = 0;
    do {
        usleep(100000);
        grab_done = wiringPiI2CRead(fd_teensy);
    } while (grab_done != 0x05);
    printf("J'ai fini d'attraper le stack");
    */

}
