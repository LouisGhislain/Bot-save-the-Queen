// FSM.h
#ifndef FSM_H
#define FSM_H

#include "Robot.h" 

// State enumdef
enum State {
    MOVING_FIRST_STACK,
    FIRST_MANEUVER,
    GRABBING,
    WAITING,
    SEPARATING, 
    STOPPED
};

extern State STATE; // 0 = blue_bottom, 1 = blue_side, 2 = yellow_bottom, 3 = yellow_side

void start_from_yellow_side(Robot *robot, GAME *game);
void start_from_blue_bottom(Robot *robot, GAME *game);
void start_from_yellow_bottom(Robot *robot, GAME *game);
void start_from_yellow_side(Robot *robot, GAME *game);

void choose_start(Robot *robot, GAME *game);

#endif // FSM_H

