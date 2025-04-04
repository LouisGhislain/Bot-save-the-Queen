#include "../include/struct.h"

Cartesian * init_cartesian() {
    Cartesian *cart = (Cartesian *)malloc(sizeof(Cartesian));
    cart->x = 0.0;
    cart->y = 0.0;
    return cart;
}

Polar * init_polar() {
    Polar *pol = (Polar *)malloc(sizeof(Polar));
    pol->angle = 0.0;
    pol->distance = 0.0;
    return pol;
}


Cluster * init_cluster() {
    Cluster *cluster = (Cluster *)malloc(sizeof(Cluster));
    cluster->point_count = 0;
    cluster->cart_points = (Cartesian **)malloc(sizeof(Cartesian *) * 100); // Allocation de mémoire pour 100 points
    cluster->pol_points = (Polar **)malloc(sizeof(Polar *) * 100); // Allocation de mémoire pour 100 points
    for (int i = 0; i < 100; i++) {
        cluster->cart_points[i] = init_cartesian();
        cluster->pol_points[i] = init_polar(); 
    }
    
    return cluster;
}

Robot * init_robot() {
    Robot *robot = (Robot *)malloc(sizeof(Robot));
    robot->cart_pos = init_cartesian();
    robot->cart_pos->x = 0.0;
    robot->cart_pos->y = 0.0;
    return robot;
}

MAP * init_map(){
    MAP *map = (MAP *)malloc(sizeof(MAP));
    map->all_map_cart = (Cartesian **)malloc(sizeof(Cartesian *) * 1500); // Allocation de mémoire pour 1500 points
    map->all_map_pol = (Polar **)malloc(sizeof(Polar *) * 1500); // Allocation de mémoire pour 1500 points
    map->all_map_count = 0;
    for (int i = 0; i < 1500; i++) {
        map->all_map_cart[i] = init_cartesian();
        map->all_map_pol[i] = init_polar();
    }
    
    map->inside_map_cart = (Cartesian **)malloc(sizeof(Cartesian *) * 1500); // Allocation de mémoire pour 1500 points
    map->inside_map_pol = (Polar **)malloc(sizeof(Polar *) * 1500); // Allocation de mémoire pour 1500 points
    map->inside_map_count = 0;    
    for (int i = 0; i < 1500; i++) {
        map->inside_map_cart[i] = init_cartesian();
        map->inside_map_pol[i] = init_polar();
    }
    
    map->clusters = (Cluster **)malloc(sizeof(Cluster *) * 10); // Allocation de mémoire pour 10 clusters
    map->cluster_count = 0;
    for (int i = 0; i < 10; i++) {
        map->clusters[i] = init_cluster();
    }
    return map;
}

GAME * init_game() {
    GAME *game = (GAME *)malloc(sizeof(GAME));
    game->map = init_map();
    game->robot = init_robot();
    return game;
}


// Lidar * init_lidar() {
//     Lidar *lidar = (Lidar *)malloc(sizeof(Lidar));
//     lidar->all_point = (Cartesian **)malloc(sizeof(Cartesian *) * 1500); // Allocation de mémoire pour 100 points
//     lidar->all_point_polar = (Polar **)malloc(sizeof(Polar *) * 1500); // Allocation de mémoire pour 100 points
//     for (int i = 0; i < 1500; i++) {
//         lidar->all_point[i] = init_cartesian();
//         lidar->all_point_polar[i] = init_polar();
//     }
    
//     lidar->clusters = (Cluster **)malloc(sizeof(Cluster *) * 10); // Allocation de mémoire pour 10 clusters
//     for (int i = 0; i < 10; i++) {
//         lidar->clusters[i] = init_cluster();
//     }
//     lidar->cluster_count = 0;
//     return lidar;
// }

// Enemy * init_enemy() {
//     Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
//     enemy->pol_pos = (Polar **)malloc(sizeof(Polar *) * 10); // Allocation de mémoire pour 10 ennemis
//     enemy->cart_pos = (Cartesian **)malloc(sizeof(Cartesian *) * 10); // Allocation de mémoire pour 10 ennemis
//     for (int i = 0; i < 10; i++) {
//         enemy->pol_pos[i] = init_polar();
//         enemy->cart_pos[i] = init_cartesian();
//     }
//     enemy->ene_pol_pos = init_polar();
//     enemy->ene_cart_pos = init_cartesian();
//     enemy->enemy_count = 0;
//     return enemy;
// }




void free_cluster(Cluster *cluster) {
    for (size_t i = 0; i < 100; i++) {
        free(cluster->cart_points[i]);
        free(cluster->pol_points[i]);
    }
    free(cluster->cart_points);
    free(cluster->pol_points);
    free(cluster);
}

// void free_lidar(Lidar *lidar) {
//     for (size_t i = 0; i < 10; i++) {
//         free_cluster(lidar->clusters[i]);
//     }
//     free(lidar->clusters);
//     free(lidar);
// }

void free_map(MAP *map) {
    for (size_t i = 0; i < 1500; i++) {
        free(map->all_map_cart[i]);
        free(map->all_map_pol[i]);
        free(map->inside_map_cart[i]);
        free(map->inside_map_pol[i]);
    }
    for (size_t i = 0; i < 10; i++) {
        free_cluster(map->clusters[i]);
    }
    free(map->all_map_cart);
    free(map->all_map_pol);
    free(map->inside_map_cart);
    free(map->inside_map_pol);
    free(map->clusters);
    free(map);
}