#include "../../include/Robot.h"
#include <iostream>
#include <unistd.h> // Pour sleep()

void Robot::screen_init() {
    fd_screen= wiringPiI2CSetup(SCREEN_ADDR);
    if (fd_screen < 0) {
        std::cerr << "Erreur : Impossible de se connecter à l'écran !" << std::endl;
    }
    screen_send_command(0x33);  // Init 8-bit mode
    delay(5);
    screen_send_command(0x32);  // Init 4-bit mode
    delay(5);
    screen_send_command(0x28);  // 2 lines, 5x7 font
    delay(5);
    screen_send_command(0x0C);  // Display on, cursor off
    delay(5);
    screen_send_command(0x01);  // Clear display
    delay(5);
    wiringPiI2CWrite(fd_screen, 0x08);  // Backlight

}

void Robot::screen_clear(){
    screen_send_command(0x01);  // Clear display
    delay(5);
}

void Robot::screen_send_command(int comm){
    int buf;

    // High nibble
    buf = comm & 0xF0;
    buf |= 0x04;  // RS=0, RW=0, EN=1
    screen_write_word(buf);
    delay(2);
    buf &= 0xFB;  // EN=0
    screen_write_word(buf);

    // Low nibble
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;
    screen_write_word(buf);
    delay(2);
    buf &= 0xFB;
    screen_write_word(buf);
}

void Robot::screen_write_word(int data) {
    int temp = data;
    temp |= 0x08;
    wiringPiI2CWrite(fd_screen, temp);
}


void Robot::screen_send_data(int data) {
    int buf;

    // High nibble
    buf = data & 0xF0;
    buf |= 0x05;  // RS=1, RW=0, EN=1
    screen_write_word(buf);
    delay(2);
    buf &= 0xFB;
    screen_write_word(buf);

    // Low nibble
    buf = (data & 0x0F) << 4;
    buf |= 0x05;
    screen_write_word(buf);
    delay(2);
    buf &= 0xFB;
    screen_write_word(buf);
}

void Robot::screen_write(int x, int y, const char data[]) {
    int addr, i;
    int tmp;

    if (x < 0)  x = 0;
    if (x > 19) x = 19;
    if (y < 0)  y = 0;
    if (y > 3)  y = 3;

    // Adresse de début pour les lignes du LCD2004
    const int row_addr[] = {0x00, 0x40, 0x14, 0x54};
    addr = 0x80 + row_addr[y] + x;
    screen_send_command(addr);

    tmp = strlen(data);
    for (i = 0; i < tmp; i++) {
        screen_send_data(data[i]);
    }
}

void Robot::screen_end_game(){
    char buffer[50];
    sprintf(buffer, "%d points", points_scored);
    screen_write(0, 0, "I am The Queen");
    screen_write(0, 1, "and I scored : ");
    screen_write(0, 3, buffer);

}

void Robot::screen_create_custom_chars() {
    uint8_t chars[6][8] = {
        // char 0 : haut gauche
        {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00110,
            0b01001,
            0b10010
        },
        // char 1 : haut milieu
        {
            0b00000,
            0b00000,
            0b01000,
            0b11100,
            0b01000,
            0b10101,
            0b01010,
            0b01001
        },
        // char 2 : haut droit
        {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b10000,
            0b01000,
            0b00100
        },
        // char 3 : bas gauche
        {
            0b10010,
            0b10010,
            0b01001,
            0b00110,
            0b00000,
            0b00111,
            0b00000,
            0b00111
        },
        // char 4 : bas milieu
        {
            0b01001,
            0b01001,
            0b01010,
            0b11101,
            0b00000,
            0b11111,
            0b00000,
            0b11111
        },
        // char 5 : bas droit
        {
            0b00100,
            0b00100,
            0b01000,
            0b10000,
            0b00000,
            0b10000,
            0b00000,
            0b10000
        }
    };

    for (int c = 0; c < 6; ++c) {
        screen_send_command(0x40 + (c * 8));  // Adresse CGRAM pour chaque caractère
        delay(2);
        for (int i = 0; i < 8; ++i) {
            screen_send_data(chars[c][i]);
            delay(2);  // délai entre chaque ligne pour fiabilité
        }
    }
}


void Robot::screen_display_intro(){
    screen_write(0, 0, "Je suis");
    screen_write(0, 1, "Bot save the Queen");

    screen_create_custom_chars();
    delay(20);
    screen_write(7, 2, "\x00");
    screen_write(8, 2, "\x01");
    screen_write(9, 2, "\x02");
    screen_write(7, 3, "\x03");
    screen_write(8, 3, "\x04");
    screen_write(9, 3, "\x05");
    return;

}
