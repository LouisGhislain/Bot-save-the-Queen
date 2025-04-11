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



void Robot::teensy_build(){
    int rasp_backward = 0;
    int rasp_forward = 0 ;
    int sequence_finished = 0 ;

    teensy_send_command(0x04);//CMD_GRAB
    usleep(800000);
    lowLevelBackward();
    usleep(10000000);
    lowLevelForward();
    teensy_send_command(0x07); //CMD_FORWARD_DONE
    usleep(1000000);
    lowLevelBackward();
    
}

void Robot::lowLevelBackward(){
    double ref_speed_left = -4;
    double ref_speed_right = -4;

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 2*1000000; // To seconds;
    unsigned long currentTime;
    unsigned long startloop;
    unsigned long looptime;

    while (micros() - startTime < duration) {
        startloop = micros();

        currentTime = micros() - startTime;
        lowLevelController();
        looptime = micros() - startloop;
        usleep(SAMPLING_TIME*1e6 - looptime);
    }
    stop();
}

void Robot::lowLevelForward(){
    double ref_speed_left = 4;
    double ref_speed_right = 4;

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 3*1000000; // To seconds;
    unsigned long currentTime;
    unsigned long startloop;
    unsigned long looptime;

    while (micros() - startTime < duration) {
        startloop = micros();

        currentTime = micros() - startTime;
        lowLevelController();
        looptime = micros() - startloop;
        usleep(SAMPLING_TIME*1e6 - looptime);
    }
    stop();
}