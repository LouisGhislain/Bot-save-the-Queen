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

#define PRE_NODE_STACK_0 10
#define PRE_NODE_STACK_1 11
#define PRE_NODE_STACK_2 12
#define PRE_NODE_STACK_3 13
#define PRE_NODE_BOTTOM_STACK_4 14
#define PRE_NODE_BOTTOM_STACK_5 16
#define PRE_NODE_TOP_STACK_4 15
#define PRE_NODE_TOP_STACK_5 17 
#define PRE_NODE_STACK_6 18
#define PRE_NODE_STACK_7 19
#define PRE_NODE_STACK_8 20
#define PRE_NODE_STACK_9 21
#define NODE_STACK_0 0 
#define NODE_STACK_1 1
#define NODE_STACK_2 2
#define NODE_STACK_3 3
#define NODE_STACK_4 4 
#define NODE_STACK_5 5
#define NODE_STACK_6 6
#define NODE_STACK_7 7
#define NODE_STACK_8 8
#define NODE_STACK_9 9

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

//----Drop banner----
void drop_banner(Robot *robot, GAME *game);
extern int STATE_DROPPING ;//State for dropping banner

//----Grab stack-----
void fsm_grab_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE);
extern int STATE_GRABBING ; //state for grabbing one stack

//----Build stack----
void fsm_build_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE);
extern int STATE_BUILDING ; 

#endif // FSM_H

