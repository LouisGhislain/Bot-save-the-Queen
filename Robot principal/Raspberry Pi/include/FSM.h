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
#define PRE_END_ZONE_YELLOW 26
#define PRE_END_ZONE_BLUE 27
#define END_ZONE_YELLOW 56
#define END_ZONE_BLUE 57

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
#define CONSTRUCTION_YELLOW_0 29
#define CONSTRUCTION_YELLOW_1 30
#define CONSTRUCTION_YELLOW_2 33
#define CONSTRUCTION_YELLOW_3 35
#define PRE_CONSTRUCTION_YELLOW_0 42
#define PRE_CONSTRUCTION_YELLOW_1 43
#define PRE_CONSTRUCTION_YELLOW_2 53
#define PRE_CONSTRUCTION_YELLOW_3 47

#define CONSTRUCTION_BLUE_0 32
#define CONSTRUCTION_BLUE_1 31
#define CONSTRUCTION_BLUE_2 28
#define CONSTRUCTION_BLUE_3 34
#define PRE_CONSTRUCTION_BLUE_0 45
#define PRE_CONSTRUCTION_BLUE_1 44
#define PRE_CONSTRUCTION_BLUE_2 52
#define PRE_CONSTRUCTION_BLUE_3 46

#define DISTANCE_NODE_PRE_NODE 0.21 // distance between the node and the pre_node to stop

// State
extern int STATE;
extern int STATE_RETURN_TO_BASE; // State for returning to base

inline constexpr int time_return_to_base = 90; // in seconds, the time at which we stop everything to return to the base
inline constexpr int time_reach_end_zone = 97;

void start_from_yellow_side(Robot *robot, GAME *game);
void start_from_blue_bottom(Robot *robot, GAME *game);
void start_from_yellow_bottom(Robot *robot, GAME *game);
void start_from_yellow_side(Robot *robot, GAME *game);

void return_to_base(Robot *robot, GAME *game);
void return_to_base_blue(Robot *robot, GAME *game);
void return_to_base_yellow(Robot *robot, GAME *game);

void choose_start(Robot *robot, GAME *game);

//----Drop banner----
void drop_banner(Robot *robot, GAME *game);
extern int STATE_DROPPING ;//State for dropping banner

//----Grab stack-----
void fsm_grab_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE);
extern int STATE_GRABBING ; //state for grabbing one stack

//----Build stack----
void fsm_build_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE);
void fsm_build_american_stage(Robot *robot, GAME *game, int PRE_NODE, int NODE);
void fsm_build_normal_third_stage(Robot *robot, GAME *game, int SEPARATE_NODE, int PRE_CONSTRUCT_NODE_1, int CONSTRUCT_NODE_1, int CONSTRUCT_NODE_2);
void fsm_build_romain(Robot *robot, GAME *game, int PRE_NODE, int NODE);
extern int STATE_BUILDING ; 

#endif // FSM_H

