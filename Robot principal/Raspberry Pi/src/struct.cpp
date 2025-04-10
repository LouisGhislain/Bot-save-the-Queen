#include "../include/struct.h"

// Initialisation functions

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
    cluster->cart_points = (Cartesian **)malloc(sizeof(Cartesian *) * Pt_cluster); // Allocation de mémoire pour 100 points
    cluster->pol_points = (Polar **)malloc(sizeof(Polar *) * Pt_cluster); // Allocation de mémoire pour 100 points
    for (int i = 0; i < Pt_cluster; i++) {
        cluster->cart_points[i] = init_cartesian();
        cluster->pol_points[i] = init_polar(); 
    }
    
    return cluster;
}

Queen * init_robot() {
    Queen *queen = (Queen *) malloc(sizeof(Queen));
    queen->cart_pos = init_cartesian();
    queen->cart_pos->x = 0.0;
    queen->cart_pos->y = 0.0;
    return queen;
}



MAP * init_map(){
    MAP *map = (MAP *)malloc(sizeof(MAP));
    map->all_map_cart = (Cartesian **)malloc(sizeof(Cartesian *) * Pt_lidar); // Allocation de mémoire pour 1500 points
    map->all_map_pol = (Polar **)malloc(sizeof(Polar *) * Pt_lidar); // Allocation de mémoire pour 1500 points
    map->all_map_count = 0;

    map->inside_map_cart = (Cartesian **)malloc(sizeof(Cartesian *) * Pt_lidar); // Allocation de mémoire pour 1500 points
    map->inside_map_pol = (Polar **)malloc(sizeof(Polar *) * Pt_lidar); // Allocation de mémoire pour 1500 points
    map->inside_map_count = 0;    

    for (int i = 0; i < Pt_lidar; i++) {
        map->all_map_cart[i] = init_cartesian();
        map->all_map_pol[i] = init_polar();

        map->inside_map_cart[i] = init_cartesian();
        map->inside_map_pol[i] = init_polar();
    }
    
    map->clusters = (Cluster **)malloc(sizeof(Cluster *) * Nb_Cluster); // Allocation de mémoire pour 10 clusters
    map->cluster_count = 0;
    for (int i = 0; i < Nb_Cluster; i++) {
        map->clusters[i] = init_cluster();
    }
    return map;
}


Stack * init_stack() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->Stack_cart = init_cartesian();
    stack->Free = true;
    return stack;
}


Target * init_target() {
    Target *target = (Target *)malloc(sizeof(Target));
    target->stack = (Stack **)malloc(sizeof(Stack *) * Nb_target); // Allocation de mémoire pour 10 cibles
    for (int i = 0; i < Nb_target; i++) {
        target->stack[i] = init_stack();
    }
    target->stack_free_count = Nb_target;
    target->stack_count = Nb_target;
    target->stack_taken_count = 0;
    return target;
}

Ennemy * init_ennemy() {
    Ennemy *enemy = (Ennemy *)malloc(sizeof(Ennemy));
    enemy->cart_pos = init_cartesian();
    enemy->cart_pos->x = 0.0;
    enemy->cart_pos->y = 0.0;
    enemy->pol_pos = init_polar();
    enemy->pol_pos->angle = 0.0;
    enemy->pol_pos->distance = 0.0;
    enemy->too_close = false;
    enemy->calibrated = false;
    return enemy;
}


GAME * init_game() {
    GAME *game = (GAME *)malloc(sizeof(GAME));
    game->map = init_map();
    game->queen = init_robot();
    game->target = init_target();
    game->Sauron = init_ennemy();

    return game;
}

// Freeing functions

void free_cluster(Cluster *cluster) {
    for (size_t i = 0; i < Pt_cluster; i++) {
        free(cluster->cart_points[i]);
        free(cluster->pol_points[i]);
    }
    free(cluster->cart_points);
    free(cluster->pol_points);
    free(cluster);
}


void free_map(MAP *map) {
    for (size_t i = 0; i < Pt_lidar; i++) {
        free(map->all_map_cart[i]);
        free(map->all_map_pol[i]);
        free(map->inside_map_cart[i]);
        free(map->inside_map_pol[i]);
    }
    for (size_t i = 0; i < Nb_Cluster; i++) {
        free_cluster(map->clusters[i]);
    }
    free(map->all_map_cart);
    free(map->all_map_pol);
    free(map->inside_map_cart);
    free(map->inside_map_pol);
    free(map->clusters);
    free(map);
}


void free_Ennemy(Ennemy *ennemy) {
    free(ennemy->cart_pos);
    free(ennemy->pol_pos);
    free(ennemy);
}

void free_Stack(Stack *stack) {
    free(stack->Stack_cart);
    free(stack);
}

void free_Target(Target *target) {
    for (size_t i = 0; i < target->stack_count; i++) {
        free_Stack(target->stack[i]);
    }
    free(target->stack);
    free(target);
}


void free_robot(Queen *Queen) {
    free(Queen->cart_pos);
    free(Queen);
}

void free_game(GAME *game) {
    free_map(game->map);
    free_robot(game->queen);
    free_Ennemy(game->Sauron);
    free(game->Sauron);
    free_Target(game->target);
    free(game);
}