#ifndef STRUCT_H
#define STRUCT_H

#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <utility>
//#include "../lib/rplidar_sdk/sdk/include/rptypes.h"
//#include "../lib/rplidar_sdk/sdk/include/sl_lidar.h"
//#include "../lib/rplidar_sdk/sdk/include/sl_lidar_driver.h"
#include <vector>
#include <tuple>
#include <optional>
#include <fstream>
#include <cmath>
#include <string>
#include <filesystem>
#include <limits>
#include <thread>

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
    int id; // ID du cluster
} Cluster;


// typedef struct Ennemy{
//     Cartesian *cart_pos; // Position cartésienne de l'ennemi
//     Polar *pol_pos; // Position polaire de l'ennemi
// } Enemy;

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

typedef struct Robot{
    Cartesian *cart_pos; // Position cartésienne du robot
    double angle; // Angle du robot
} Robot;

typedef struct GAME{
    MAP *map; // Carte du jeu
    Robot *robot; // Robot du jeu
    
} GAME;


Cartesian * init_cartesian();
Polar * init_polar();
Cluster * init_cluster();
MAP * init_map();
GAME * init_game();


void free_cluster(Cluster *cluster);
void free_map(MAP *map);


#endif // STRUCT_H