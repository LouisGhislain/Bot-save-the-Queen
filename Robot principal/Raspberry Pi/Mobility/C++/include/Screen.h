#ifndef SCREEN_H
#define SCREEN_H

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string>

#define OLED_ADDR 0x3C  // Adresse I2C de l'OLED
#define OLED_CMD  0x00  // Mode commande
#define OLED_DATA 0x40  // Mode données

class Screen {
public:
    Screen();  // Constructeur
    ~Screen(); // Destructeur

    void init();         // Initialisation de l'écran
    void clear();        // Efface l'écran
    void displayText(const std::string &text); // Affiche du texte

private:
    int fd; // File descriptor pour la communication I2C
};

#endif // SCREEN_H
