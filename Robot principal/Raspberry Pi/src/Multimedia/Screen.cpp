#include "../../include/Robot.h"
#include <iostream>
#include <unistd.h> // Pour sleep()

void Robot::screen_init() {
    fd_OLED = wiringPiI2CSetup(OLED_ADDR);
    if (fd_OLED < 0) {
        std::cerr << "Erreur : Impossible de se connecter à l'écran OLED !" << std::endl;
    }
    
    // Séquence d'initialisation du SSD1306
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xAE); // Désactive l'affichage
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xD5);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x80);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xA8);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x3F);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xD3);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x00);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x40);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x8D);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x14);
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xAF); // Active l'affichage
}

void Robot::screen_clear() {
    if (fd_OLED < 0) return;
    
    for (int i = 0; i < 8; i++) {
        wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xB0 + i);
        wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x00);
        wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x10);
        for (int j = 0; j < 128; j++) {
            wiringPiI2CWriteReg8(fd_OLED, OLED_DATA, 0x00);
        }
    }
}

void Robot::screen_displayText(const std::string &text) {
    if (fd_OLED < 0) return;
    
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0xB0); // Ligne 0
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x00); // Colonne 0
    wiringPiI2CWriteReg8(fd_OLED, OLED_CMD, 0x10);
    
    for (char c : text) {
        wiringPiI2CWriteReg8(fd_OLED, OLED_DATA, c);
    }
}