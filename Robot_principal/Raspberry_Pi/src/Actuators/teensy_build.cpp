#include "../../include/Robot.h"

void Robot::teensy_build(void *game){
    build_finished = false;
    teensy_send_command(0x04);   // ENVOIE LA COMMANDE   (CMD_BUILD)

    usleep(0.7*1000000);         // RELEASE LES EXTERIEURS    // previous 0.4

    straightMotion(-0.12,game);  // RECULE AVEC LES CREMAILLERES

    usleep(4*1000000);             // MONTE LE STACK AU 2e ETAGE // previous 3.5

    straightMotion(0.12,game);   // AVANCE AVEC STACK HAUT

    usleep(2000000);             // RELEASE LE STACK AU 2e ETAGE
    
    straightMotion(-0.1,game);   // RECULE POUR NE PAS CASSER LE GRADIN

                                 // RABAISSE LE HOLD CANS AU 1er ETAGE
    
    build_finished = true;  

}

void Robot::teensy_push_separate(void *game){
    separate_finished = false;
    teensy_send_command(0x05);
    straightMotion(0.12,game);
    usleep(2000000); //reculer
    straightMotion(-0.12,game);
    separate_finished = true ; 
}


/*
    int rasp_backward = 0;
    int rasp_forward = 0 ;
    do {
        usleep(10000);  // Attendre 100 ms entre chaque lecture
        rasp_forward = wiringPiI2CRead(fd_teensy);
    } while (rasp_forward != 0x06);
     
*/
