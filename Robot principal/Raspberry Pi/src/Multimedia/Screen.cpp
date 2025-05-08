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
    std::cout << "Screen initialized" << std::endl; 
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
    screen_clear(); // Clear previous data

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%d points", points_scored);

    screen_write(0, 0, "I am The Queen");
    screen_write(0, 1, "and I scored : ");
    screen_write(0, 3, buffer);
    std::cout << "Points scored: " << points_scored << std::endl;

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

void Robot::screen_menu(GAME *game) {
    const char* teams[] = {"BLEU", "JAUNE"};
    const char* positions[] = {"BAS", "COTE"};

    int team_index = 0;
    int pos_index = 0;
    int previous_selected_team = -1 ; 
    int previous_selected_start = -1 ; 

menu_start:  // <-- Point de redémarrage du menu

    // ÉTAPE 1 : Choix de l'équipe
    while (true) {
        if(selected_team!=previous_selected_team){
                screen_clear();
                screen_write(0, 0, "Team :");
                screen_write(8, 0, teams[0]);
                screen_write(8, 1, teams[1]);
                // Affiche un curseur '>' devant la sélection
                screen_write(6, team_index, ">");
                previous_selected_team = selected_team;
        }

        delay(200);
        if (is_left_pressed(game)) {
            team_index = 1 - team_index;
            selected_team = (selected_team + 1) % 2;
            delay(300);  // Anti-rebond
        }
        if (is_right_pressed(game)) {
            delay(300);
            break;
        }
    }

    // ÉTAPE 2 : Choix de la position de départ de l'adversaire
    while (true) {
        if(opponent_start!=previous_selected_start){
            screen_clear();
            screen_write(0, 0, "Depart de l'autre");
            screen_write(0, 1, "equipe :");
            screen_write(8, 2, positions[0]);
            screen_write(8, 3, positions[1]);
            screen_write(6, 2 + pos_index, ">");
            previous_selected_start = opponent_start ;
        }

        delay(200);
        if (is_left_pressed(game)) {
            pos_index = 1 - pos_index;
            opponent_start = (opponent_start + 1) % 2;
            delay(300);
        }
        if (is_right_pressed(game)) {
            delay(300);
            break;
        }
    }

    // ÉTAPE 3 : Résumé + confirmation
    bool confirmed = false;
    bool last_display = false;
    
    while (!confirmed) {
        if (!last_display) {
            screen_clear();
            char line1[21];
            char line2[21];
            snprintf(line1, sizeof(line1), "Je suis team %s", selected_team == 0 ? "BLEU " : "JAUNE");
            snprintf(line2, sizeof(line2), "Adversaire : %s", opponent_start == 0 ? "BAS" : "COTE");
            screen_write(0, 0, line1);
            screen_write(0, 1, line2);
            screen_write(0, 3, "Valider ?");
            last_display = true;
        }
    
        if (is_right_pressed(game)) {
            confirmed = true;  // fin du menu
        }
    
        if (is_left_pressed(game)) {
            // Revenir au début
            screen_clear();
            screen_menu(game);  // Redémarre tout le menu
            return;
        }
    
        delay(100); // Petit délai pour éviter boucle trop rapide
    }
    

    // Sauvegarde les choix si besoin
    selected_team = team_index;      // 0 = BLEU, 1 = JAUNE
    opponent_start = pos_index;      // 0 = BAS, 1 = CÔTÉ
}
