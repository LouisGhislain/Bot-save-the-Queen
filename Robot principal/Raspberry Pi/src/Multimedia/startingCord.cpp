#include "../../include/Robot.h"

void Robot::wait_starting_cord(GAME *game) {
    // Wait for the starting cord to be inserted
    // starting pin inserted : HIGH
    // starting pin absent   : LOW
    if (digitalRead(STARTING_CORD_PIN) == HIGH) { // cord not inserted
        fprintf(stderr, "--------------------------------------------------------\n");
        fprintf(stderr, "Insert the starting cord in the robot !\n");
        fprintf(stderr, "Waiting insertion ...\n");
        fprintf(stderr, "--------------------------------------------------------\n");
    }
    while(digitalRead(STARTING_CORD_PIN) == HIGH) {
        usleep(300000); // Sleep for 0.3 seconds
    }

    if (digitalRead(STARTING_CORD_PIN) == LOW) { // cord inserted
        fprintf(stderr, "--------------------------------------------------------\n");
        fprintf(stderr, "Starting cord inserted !\n");
        fprintf(stderr, "Waiting starting cord release for starting ...\n");
        fprintf(stderr, "--------------------------------------------------------\n");
        usleep(2000000); // Sleep for 2 seconds to avoid inserting issues
    }
    while(digitalRead(STARTING_CORD_PIN) == LOW) { // cord inserted
        usleep(50000); // Sleep for 0.05 seconds
    }

    // if we go out of the while, starting cord is released
    game->starting_MATCH_TIME = std::chrono::steady_clock::now(); // set the actual time to the starting time
    fprintf(stderr, "STARTING !\n");
    fprintf(stderr, "--------------------------------------------------------\n");
}