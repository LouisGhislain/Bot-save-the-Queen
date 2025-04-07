#ifndef STRUCT_H
#define STRUCT_H

#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <utility>
#include "../lib/rplidar_sdk/sdk/include/rptypes.h"
#include "../lib/rplidar_sdk/sdk/include/sl_lidar.h"
#include "../lib/rplidar_sdk/sdk/include/sl_lidar_driver.h"
#include <vector>
#include <tuple>
#include <optional>
#include <fstream>
#include <cmath>
#include <string>
#include <filesystem>
#include <limits>
#include <thread>

#define ennemy_count 3  // Nombre d'ennemis
#define Pt_cluster 1500 // Nombre de points par cluster
#define Pt_lidar 1500 // Nombre de points par LIDAR
#define Nb_Cluster 10 // Nombre de clusters
#define Nb_target 10 // Nombre de cibles


typedef struct Cartesian{
    double x; // Coordonnée x
    double y; // Coordonnée y
} Cartesian;

typedef struct Polar{
    double angle; // Angle en radians
    double distance; // Distance
} Polar;


typedef struct Cluster{
    Cartesian** cart_points; // Points du cluster
    Polar** pol_points; // Points polaires du cluster
    size_t point_count; // Nombre de points dans le cluster
} Cluster;


typedef struct Ennemy{
    Cartesian *cart_pos; // Position cartésienne de l'ennemi
    Polar *pol_pos; // Position polaire de l'ennemi
    bool too_close; // Indique si l'ennemi est trop proche
    int calibrated; // Indique si l'ennemi est calibré
} Ennemy;

typedef struct Stack{
    Cartesian *Stack_cart; // Position cartésienne de la cible
    bool Free; // Indique si la cible est libre
} Stack;

typedef struct Target{
    Stack **stack; // Pile de cibles
    size_t stack_free_count; // Nombre de cibles libres
    size_t stack_count; // Nombre de cibles
    size_t stack_taken_count; // Nombre de cibles prises
} Target;

typedef struct MAP{
    Cartesian **all_map_cart; // Position cartésienne du LIDAR
    Polar **all_map_pol; // Position polaire du LIDAR
    size_t all_map_count; // Nombre de points dans la carte

    Cartesian **inside_map_cart; // Position cartésienne de la carte
    Polar **inside_map_pol;  // Position polaire de la carte
    size_t inside_map_count; // Nombre de points dans la carte

    Cluster ** clusters; // Liste des clusters
    size_t cluster_count; // Nombre de clusters
} MAP;

typedef struct Queen{
    Cartesian *cart_pos; // Position cartésienne du robot
    double angle; // Angle du robot
} Queen;

typedef struct GAME{
    MAP *map; // Carte du jeu
    Queen *queen; // Robot du jeu
    Ennemy *Sauron; // Ennemi du jeu
    Target *target; // Cible du jeu
    
} GAME;


Cartesian * init_cartesian();
Polar * init_polar();
Cluster * init_cluster();
MAP * init_map();
GAME * init_game();
Queen * init_robot();
Ennemy * init_ennemy();
Stack * init_stack();
Target * init_target();


void free_game(GAME *game);
void free_Stack(Stack *stack);
void free_Target(Target *target);
void free_robot(Queen *queen);
void free_Ennemy(Ennemy *ennemy);
void free_cluster(Cluster *cluster);
void free_map(MAP *map);


#endif // STRUCT_H
