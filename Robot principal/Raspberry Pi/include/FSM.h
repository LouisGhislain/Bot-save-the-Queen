// FSM.h
#ifndef FSM_H
#define FSM_H

#include "Robot.h" 

// POSITION ET NUMEROTATION DES STACKS
//   ______________________________
//  |          |  MAP  |          |
//  |        8 |_______| 9        |
//  |  6                       7  |
//  |           4     5           |
//  |  2                       3  |
//  |       0             1       |
//  |_____________________________|
//
// préciser ici le positionnement des stacks

#define END_ZONE_YELLOW 
#define END_ZONE_BLUE

#define PRE_NODE_STACK_0 7
#define PRE_NODE_STACK_1
#define PRE_NODE_STACK_2
#define PRE_NODE_STACK_3
#define PRE_NODE_STACK_4 1
#define PRE_NODE_STACK_5
#define PRE_NODE_STACK_6
#define PRE_NODE_STACK_7
#define PRE_NODE_STACK_8
#define PRE_NODE_STACK_9
#define NODE_STACK_0 8
#define NODE_STACK_1
#define NODE_STACK_2
#define NODE_STACK_3
#define NODE_STACK_4 0
#define NODE_STACK_5
#define NODE_STACK_6
#define NODE_STACK_7
#define NODE_STACK_8
#define NODE_STACK_9

// position des zones de construction
//   ______________________________
//  |          |  MAP  |          |
//  |          |_______|          |
//  |                             |
//  | B3                       Y3 |
//  |                             |
//  |                             |
//  |_B2___Y0__Y1_____B1__B0___Y2_|
//
#define CONSTRUCTION_YELLOW_0 
#define CONSTRUCTION_YELLOW_1 5
#define CONSTRUCTION_YELLOW_2
#define CONSTRUCTION_YELLOW_3
#define PRE_CONSTRUCTION_YELLOW_0
#define PRE_CONSTRUCTION_YELLOW_1 4
#define PRE_CONSTRUCTION_YELLOW_2
#define PRE_CONSTRUCTION_YELLOW_3

#define CONSTRUCTION_BLUE_0
#define CONSTRUCTION_BLUE_1
#define CONSTRUCTION_BLUE_2
#define CONSTRUCTION_BLUE_3
#define PRE_CONSTRUCTION_BLUE_0
#define PRE_CONSTRUCTION_BLUE_1
#define PRE_CONSTRUCTION_BLUE_2
#define PRE_CONSTRUCTION_BLUE_3

// State
extern int STATE;

void start_from_yellow_side(Robot *robot, GAME *game);
void start_from_blue_bottom(Robot *robot, GAME *game);
void start_from_yellow_bottom(Robot *robot, GAME *game);
void start_from_yellow_side(Robot *robot, GAME *game);

void choose_start(Robot *robot, GAME *game);

#endif // FSM_H

