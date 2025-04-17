#include "../../include/Robot.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#define TEENSY_ADDR 0x08

void Robot::teensy_init(){
    fd_teensy= wiringPiI2CSetup(TEENSY_ADDR);
    if (fd_teensy < 0) {
        std::cerr << "Erreur : Impossible de se connecter à la Teensy !" << std::endl;
    }
}


void Robot::teensy_send_command(uint8_t command){
    const char *filename = "/dev/i2c-1";
    int fd_teensy = open(filename, O_RDWR);
    if (fd_teensy < 0) {
        perror("Failed to open the i2c bus");
        return;
    }

    if (ioctl(fd_teensy, I2C_SLAVE, TEENSY_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(fd_teensy);
        return;
    }

    // Write only one byte (the command)
    if (write(fd_teensy, &command, 1) != 1) {
        perror("Failed to write to the i2c bus");
    } else {
        std::cout << "Commande envoyée avec succès." << std::endl;
    }

    close(fd_teensy);
}