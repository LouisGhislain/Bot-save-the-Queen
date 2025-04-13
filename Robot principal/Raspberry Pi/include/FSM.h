// FSM.h
#ifndef FSM_H
#define FSM_H

#include "Robot.h" 

// State
extern int STATE;

void start_from_yellow_side(Robot *robot, GAME *game);
void start_from_blue_bottom(Robot *robot, GAME *game);
void start_from_yellow_bottom(Robot *robot, GAME *game);
void start_from_yellow_side(Robot *robot, GAME *game);

void choose_start(Robot *robot, GAME *game);

#endif // FSM_H

