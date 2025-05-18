#ifndef LIDAR_H
#define LIDAR_H

#include "struct.h"
#include "../lib/rplidar_sdk/sdk/include/rptypes.h"  // Le fichier rptypes.h doit être inclus en premier
#include "../lib/rplidar_sdk/sdk/include/sl_lidar.h"
#include "../lib/rplidar_sdk/sdk/include/rplidar_cmd.h"
#include "../lib/rplidar_sdk/sdk/include/rplidar_driver.h"
#include "../lib/rplidar_sdk/sdk/include/sl_types.h"
#include <fstream> // Pour écrire dans un fichier
#include <iostream>
#include <vector>
#include <queue>
#include <cmath> // pour M_PI


using namespace sl;

// Structure représentant un objet avec ses coordonnées
struct Objet {
    std::vector<double> x;
    std::vector<double> y;
    double x_mean;
    double y_mean;
};

// Structure pour représenter une balise
struct Beacon {
    double x;
    double y;
};

struct Triangle {
    size_t i;
    size_t j;
    size_t k;
};

struct PolarCoord {
    double angle; // en radians
    double distance;
    int label;
};

static constexpr double EPS = 0.1;
static constexpr int MIN_PTS = 1;
static constexpr double STOP_DISTANCE_ENNEMY_FRONT = 0.65; // Distance d'arrêt en m
static constexpr double STOP_ANGLE_ENNEMY = 50; // Angle d'arrêt en radians donc si ennemi dans la fenêtre -20° +20° par rapport au robot
static constexpr double STOP_DISTANCE_ENNEMY_SIDE = 0.20;
static constexpr double coef_detection_profile = STOP_DISTANCE_ENNEMY_FRONT/STOP_DISTANCE_ENNEMY_SIDE - 1; // coefficient de détection de l'ennemi 
// = STOP_DISTANCE_ENNEMY_FRONT/STOP_DISTANCE_ENNEMY_SIDE - 1 = 0.6666666666666667


// Fonction pour se connecter au LIDAR
void init_connectLidar();
void print_Sauron_position(GAME *game);
void fetchLidarData(void * sqid_void);
void clustering_nearest(GAME * squid);
void Emergency_stop(GAME* squid);
void clustering_calibrated(GAME * squid);
void pos_of_stack(GAME * squid);
void stack_is_taking_by_ennemy(GAME* squid);
void Path_updating(GAME* squid);
void print_free_nodes(GAME* squid);

#endif // LIDAR_H