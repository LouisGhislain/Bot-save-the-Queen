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
static constexpr double STOP_DISTANCE_ENNEMY = 500; // Distance d'arrêt en mm



// Fonction pour se connecter au LIDAR
void init_connectLidar();
void print_Sauron_position(GAME *game);
void fetchLidarData(void * sqid_void);
void clustering_nearest(GAME * squid);
void Emergency_stop(GAME* squid);
void clustering_calibrated(GAME * squid);


#endif // LIDAR_H