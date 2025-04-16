#include "../../include/lidar.h"

using namespace sl;

ILidarDriver* lidardriver;

void init_connectLidar(){

  IChannel* _channel;
  lidardriver = *createLidarDriver();
  _channel = (*createSerialPortChannel("/dev/ttyUSB0", 115200));
    if (SL_IS_OK((lidardriver)->connect(_channel))){
        printf("Connected\n");

        lidardriver->setMotorSpeed();
        sl_lidar_response_measurement_node_hq_t scanMode;;
        lidardriver->startScan(false, true);
    }
    else {
      printf("Connection failed\n");
    }
}

void print_Sauron_position(GAME *game){
  // get position of the adversory
  if (game->map->cluster_count > 0) {
    std::cout << "Position Sauron: (" << game->Sauron->cart_pos->x << ", " 
              << game->Sauron->cart_pos->y << ")" << std::endl;
    // Emergency_stop(game);
  }
  else{
    //fprintf(stderr, "No oppponent detected !\n");
  }
}


void Find_Point_Map(void * sqid_void) {
  GAME * squid = (GAME *)sqid_void; // Cast du paramètre en GAME

  for (size_t i = 0; i < squid->map->all_map_count; i++) {
    // printf("Point %zu: X: %f, Y: %f\n", i, squid->map->all_map_cart[i]->x, squid->map->all_map_cart[i]->y);
    if (squid->map->all_map_cart[i]->x > 50 && squid->map->all_map_cart[i]->x < 2950 && squid->map->all_map_cart[i]->y > 50 && squid->map->all_map_cart[i]->y < 1950) {
      squid->map->inside_map_cart[squid->map->inside_map_count]->x = squid->map->all_map_cart[i]->x;
      squid->map->inside_map_cart[squid->map->inside_map_count]->y = squid->map->all_map_cart[i]->y;
      squid->map->inside_map_pol[squid->map->inside_map_count]->angle = squid->map->all_map_pol[i]->angle;
      squid->map->inside_map_pol[squid->map->inside_map_count]->distance = squid->map->all_map_pol[i]->distance;
      squid->map->inside_map_count++;
      // printf("Point inside map: (%f, %f)\n", squid->map->inside_map_cart[squid->map->inside_map_count - 1]->x, squid->map->inside_map_cart[squid->map->inside_map_count - 1]->y);
    }
    else {
      // printf("Point outside map: (%f, %f)\n", squid->map->all_map_cart[i]->x, squid->map->all_map_cart[i]->y);
    }
  }

  // printf("Inside Map Count: %zu\n", squid->map->inside_map_count);

}

void clustering_nearest(GAME * squid) {
  // Clustering of points
  double cluster_threshold = 100.0; // Distance threshold for clustering
  double last_x = INFINITY; double last_y = INFINITY;

  for (int i = 0; i < squid->map->inside_map_count; i++) {
    double x = squid->map->inside_map_cart[i]->x;
    double y = squid->map->inside_map_cart[i]->y;
    if (fabs(x - last_x) > cluster_threshold || fabs(y - last_y) > cluster_threshold) {
      squid->map->cluster_count++;
      squid->map->clusters[squid->map->cluster_count - 1]->point_count = 0;
      // printf("New cluster %d: (%f, %f)\n", squid->map->cluster_count - 1, x, y);
    } else {
      // Update the existing cluster
      squid->map->clusters[squid->map->cluster_count - 1]->cart_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->x = x;
      squid->map->clusters[squid->map->cluster_count - 1]->cart_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->y = y;
      squid->map->clusters[squid->map->cluster_count - 1]->pol_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->angle = squid->map->inside_map_pol[i]->angle;
      squid->map->clusters[squid->map->cluster_count - 1]->pol_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->distance = squid->map->inside_map_pol[i]->distance;
      squid->map->clusters[squid->map->cluster_count - 1]->point_count++;
      // printf("Adding to cluster %d: (%f, %f)\n", squid->map->cluster_count - 1, x, y);
    }
    last_x = x;
    last_y = y;
  if (squid->map->cluster_count > 0) {
    double min_distance = INFINITY;
    int closest_cluster_index = -1;

    for (int c = 0; c < squid->map->cluster_count; c++) {
      double mean_x = 0.0;
      double mean_y = 0.0;
      double mean_angle = 0.0;
      double mean_distance = 0.0;
      double pos_prece_x = INFINITY;
      double pos_prece_y;


      for (int i = 0; i < squid->map->clusters[c]->point_count; i++) {
        mean_x += squid->map->clusters[c]->cart_points[i]->x;
        mean_y += squid->map->clusters[c]->cart_points[i]->y;
        mean_angle += squid->map->clusters[c]->pol_points[i]->angle;
        mean_distance += squid->map->clusters[c]->pol_points[i]->distance;
      }

      mean_x /= squid->map->clusters[c]->point_count;
      mean_y /= squid->map->clusters[c]->point_count;
      mean_angle /= squid->map->clusters[c]->point_count;
      mean_distance /= squid->map->clusters[c]->point_count;

      if (mean_distance < min_distance) {
        min_distance = mean_distance;
        closest_cluster_index = c;
        {
          std::lock_guard<std::mutex> lock(squid->Sauron->sauron_mutex);
          squid->Sauron->cart_pos->x = mean_x;
          squid->Sauron->cart_pos->y = mean_y;
          squid->Sauron->pol_pos->angle = mean_angle;
          squid->Sauron->pol_pos->distance = mean_distance;
        }
      }

      // double error_x = mean_x - squid->Sauron->cart_pos->x;
      // double error_y = mean_y - squid->Sauron->cart_pos->y;
      // double pos_error = sqrt(error_x * error_x + error_y * error_y);

      // if (!squid->Sauron->calibrated){
      //   if (mean_distance < min_distance) {
      //     min_distance = mean_distance;
      //     closest_cluster_index = c;
  
      //     squid->Sauron->cart_pos->x = mean_x;
      //     squid->Sauron->cart_pos->y = mean_y;
      //     squid->Sauron->pol_pos->angle = mean_angle;
      //     squid->Sauron->pol_pos->distance = mean_distance;
      //     squid->Sauron->calibrated = true;
      //   }

      // }
      // else{
      //   if (pos_error < 150){
      //     squid->Sauron->cart_pos->x = mean_x;
      //     squid->Sauron->cart_pos->y = mean_y;
      //     squid->Sauron->pol_pos->angle = mean_angle;
      //     squid->Sauron->pol_pos->distance = mean_distance;
      //   } 

      // }
      }
    }
  }
}

void Sauron_finder(GAME *squid){
  // Find the nearest point in the map to the robot position
  double min_distance = INFINITY;
  int nearest_point_index = -1;

  for (int i = 0; i < squid->map->inside_map_count; i++) {
    double x = squid->map->inside_map_cart[i]->x;
    double y = squid->map->inside_map_cart[i]->y;
    double distance = sqrt(pow(x - squid->queen->cart_pos->x*1000, 2) + pow(y - squid->queen->cart_pos->y*1000, 2));

    if (distance < min_distance) {
      min_distance = distance;
      nearest_point_index = i;
    }
  }

  if (nearest_point_index != -1) {
    squid->Sauron->cart_pos->x = squid->map->inside_map_cart[nearest_point_index]->x;
    squid->Sauron->cart_pos->y = squid->map->inside_map_cart[nearest_point_index]->y;
    squid->Sauron->pol_pos->angle = squid->map->inside_map_pol[nearest_point_index]->angle;
    squid->Sauron->pol_pos->distance = squid->map->inside_map_pol[nearest_point_index]->distance;
  }


  //printf("Nearest point: (%f, %f)\n", squid->Sauron->cart_pos->x, squid->Sauron->cart_pos->y);
  //printf("Angle: %f, Distance: %f\n", squid->Sauron->pol_pos->angle, squid->Sauron->pol_pos->distance);
}

void clustering_calibrated(GAME * squid) {
  // Clustering of points
  double cluster_threshold = 100.0; // Distance threshold for clustering
  double last_x = INFINITY; double last_y = INFINITY;

  for (int i = 0; i < squid->map->inside_map_count; i++) {
    double x = squid->map->inside_map_cart[i]->x;
    double y = squid->map->inside_map_cart[i]->y;
    if (fabs(x - last_x) > cluster_threshold || fabs(y - last_y) > cluster_threshold) {
      squid->map->cluster_count++;
      squid->map->clusters[squid->map->cluster_count - 1]->point_count = 0;
      // printf("New cluster %d: (%f, %f)\n", squid->map->cluster_count - 1, x, y);
    } else {
      // Update the existing cluster
      squid->map->clusters[squid->map->cluster_count - 1]->cart_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->x = x;
      squid->map->clusters[squid->map->cluster_count - 1]->cart_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->y = y;
      squid->map->clusters[squid->map->cluster_count - 1]->pol_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->angle = squid->map->inside_map_pol[i]->angle;
      squid->map->clusters[squid->map->cluster_count - 1]->pol_points[squid->map->clusters[squid->map->cluster_count - 1]->point_count]->distance = squid->map->inside_map_pol[i]->distance;
      squid->map->clusters[squid->map->cluster_count - 1]->point_count++;
      // printf("Adding to cluster %d: (%f, %f)\n", squid->map->cluster_count - 1, x, y);
    }
    last_x = x;
    last_y = y;
  if (squid->map->cluster_count > 0) {

    for (int c = 0; c < squid->map->cluster_count; c++) {
      double mean_x = 0.0;
      double mean_y = 0.0;
      double mean_angle = 0.0;
      double mean_distance = 0.0;
      double pos_prece_x = INFINITY;
      double pos_prece_y;


      for (int i = 0; i < squid->map->clusters[c]->point_count; i++) {
        mean_x += squid->map->clusters[c]->cart_points[i]->x;
        mean_y += squid->map->clusters[c]->cart_points[i]->y;
        mean_angle += squid->map->clusters[c]->pol_points[i]->angle;
        mean_distance += squid->map->clusters[c]->pol_points[i]->distance;
      }

      mean_x /= squid->map->clusters[c]->point_count;
      mean_y /= squid->map->clusters[c]->point_count;
      mean_angle /= squid->map->clusters[c]->point_count;
      mean_distance /= squid->map->clusters[c]->point_count;
      {
        std::lock_guard<std::mutex> lock(squid->Sauron->sauron_mutex);
        double error_x = mean_x - squid->Sauron->cart_pos->x;
        double error_y = mean_y - squid->Sauron->cart_pos->y;
  
        double pos_error = sqrt(error_x * error_x + error_y * error_y);
  
        if (pos_error < 100) {
          squid->Sauron->cart_pos->x = mean_x;
          squid->Sauron->cart_pos->y = mean_y;
          squid->Sauron->pol_pos->angle = mean_angle;
          squid->Sauron->pol_pos->distance = mean_distance;
        }
      }
      
      }
    }
  }
}

// pour le multistread il faut un void en parametre de la fonction
void fetchLidarData(void * sqid_void) {

  GAME * squid = (GAME *)sqid_void; // Cast du paramètre en GAME

  const size_t max_node_count = 8192; // Taille maximale de buffer
  sl_lidar_response_measurement_node_hq_t nodes[max_node_count];
  size_t count = max_node_count;

  sl_result result = lidardriver->grabScanDataHq(nodes, count);
  
  squid->map->all_map_count = 0;
  squid->map->inside_map_count = 0;
  squid->map->cluster_count = 0;

  //Reset all point count in cluster
  for (int i = 0; i < 10; i++) {
    squid->map->clusters[i]->point_count = 0;
  }

  for (size_t i = 0; i < count; ++i) {
    double angle = nodes[i].angle_z_q14 * 90.f / (1 << 14); // Angle en degrés
    angle = angle * M_PI / 180.0;
    angle = angle - M_PI; // Convertir en radians et ajuster l'angle
    double distance = nodes[i].dist_mm_q2 / 4.0f;          // Distance en mm

    if (distance > 0 && distance < 3900) { // 3900
      double local_cart_pos_x, local_cart_pos_y, local_robot_angle;
      {
        std::lock_guard<std::mutex> lock(squid->queen->position_mutex);
        local_robot_angle = squid->queen->angle;
        local_cart_pos_x = squid->queen->cart_pos->x*1000 - 50 * cos(squid->queen->angle); // 70 est la distance entre le lidar et le robot
        local_cart_pos_y = squid->queen->cart_pos->y*1000 - 50 * sin(squid->queen->angle); // 70 est la distance entre le lidar et le robot
      }
      //fprintf(stderr, "robot x: %f , y: %f, angle: %f\n", local_cart_pos_x, local_cart_pos_y, local_robot_angle);
      squid->map->all_map_pol[squid->map->all_map_count]->angle = angle; // Convertir en radians
      squid->map->all_map_pol[squid->map->all_map_count]->distance = distance; // Distance en mm
      squid->map->all_map_cart[squid->map->all_map_count]->x = local_cart_pos_x + distance * cos(-angle + local_robot_angle); // Convertir en coordonnées cartésiennes
      squid->map->all_map_cart[squid->map->all_map_count]->y = local_cart_pos_y + distance * sin(-angle + local_robot_angle); // Convertir en coordonnées cartésiennes
      
      squid->map->all_map_count++;
      // printf("Point %zu: Angle: %f, Distance: %f, X: %f, Y: %f\n", i, angle, distance, squid->map->all_map_cart[0]->x, squid->map->all_map_cart[0]->y);

    }
  }
  
 //  printf("All Map Count: %zu\n", squid->map->all_map_count);

  //Print first element of all_map_cart and all_map_pol
 //  printf("First Point: X: %f, Y: %f, Angle: %f, Distance: %f\n", squid->map->all_map_cart[0]->x, squid->map->all_map_cart[0]->y, squid->map->all_map_pol[0]->angle, squid->map->all_map_pol[0]->distance);
  
  Find_Point_Map(squid); // Vérifier si les points sont dans la carte
  // if (!squid->Sauron->calibrated){
  //   clustering_nearest(squid); // Clustering des points
  //   squid->Sauron->calibrated = true; // Calibrer Sauron
  // }
  // else{
  //   clustering_calibrated(squid); // Clustering des points
  // }

  Sauron_finder(squid); // Trouver le point le plus proche et c'est Sauron
  stack_is_taking_by_ennemy(squid); // Vérifier si un ennemi prend une stack
  Path_updating(squid); // Mettre à jour le chemin
  // print_free_nodes(squid); // Afficher les noeuds libres

}

  
void Emergency_stop(GAME* squid) {
  // if Sauron is too cloose
  if (fabs(squid->Sauron->pol_pos->distance) < STOP_DISTANCE_ENNEMY*1000) {
    printf("Emergency stop\n");
    // Stop the robot
    squid->Sauron->too_close = true;
  }
}

void pos_of_stack(GAME* squid) {
  // Start from index 0 instead of 1 or check if you really want to skip index 0
  for (int i = 0; i < squid->target->stack_count; i++) {
    // Fix comparison operators (use == instead of =)
    if (i == 0) {
      squid->target->stack[i]->Stack_cart->x = 775;
      squid->target->stack[i]->Stack_cart->y = 250;
      squid->target->stack[i]->angle = M_PI/2;
      // (626.21, 359.324)
    }
    else if (i == 1) {
      squid->target->stack[i]->Stack_cart->x = 2225;
      squid->target->stack[i]->Stack_cart->y = 250;
      squid->target->stack[i]->angle = M_PI/2;
    }
    else if (i == 2) {
      squid->target->stack[i]->Stack_cart->x = 75;
      squid->target->stack[i]->Stack_cart->y = 400;
      squid->target->stack[i]->angle = 0;
    }
    else if (i == 3) {
      squid->target->stack[i]->Stack_cart->x = 2925;
      squid->target->stack[i]->Stack_cart->y = 400;
      squid->target->stack[i]->angle = 0;
    }
    else if (i == 4) {
      squid->target->stack[i]->Stack_cart->x = 1100;
      squid->target->stack[i]->Stack_cart->y = 950;
      squid->target->stack[i]->angle = M_PI/2;
    }
    else if (i == 5) {
      squid->target->stack[i]->Stack_cart->x = 1900;
      squid->target->stack[i]->Stack_cart->y = 950;
      squid->target->stack[i]->angle = M_PI/2;
    }
    else if (i == 6) {
      squid->target->stack[i]->Stack_cart->x = 75;
      squid->target->stack[i]->Stack_cart->y = 1325;
      squid->target->stack[i]->angle = 0;
    }
    else if (i == 7) {
      squid->target->stack[i]->Stack_cart->x = 2925;
      squid->target->stack[i]->Stack_cart->y = 1325;
      squid->target->stack[i]->angle = 0;
    }
    else if (i == 8) {
      squid->target->stack[i]->Stack_cart->x = 825;
      squid->target->stack[i]->Stack_cart->y = 1725;
      squid->target->stack[i]->angle = M_PI/2;
    }
    else if (i == 9) {
      squid->target->stack[i]->Stack_cart->x = 2175;
      squid->target->stack[i]->Stack_cart->y = 1725;
      squid->target->stack[i]->angle = M_PI/2;
    }
    else {
      printf("Error in stack position: index %d out of range\n", i);
    }
  }
}

void stack_is_taking_by_ennemy(GAME* squid) {
  for (int i = 0; i < squid->target->stack_count; i++) {
    // printf("Checking stack %d\n", i);
    // Only check stacks that are currently free
    if (squid->target->stack[i]->Free == false) {
      // printf("Stack %d is already taken\n", i);
    }
    else {
      // printf("Stack %d is free\n", i);
    }

    if (squid->target->stack[i]->Free) {
      // Store starting time outside the loop
      
      // Check proximity between stack and enemy
      double dx = squid->target->stack[i]->Stack_cart->x - squid->Sauron->cart_pos->x;
      double dy = squid->target->stack[i]->Stack_cart->y - squid->Sauron->cart_pos->y;

      // If enemy is close to the stack
      if (sqrt(dx * dx + dy * dy) < 350) {
        auto now = std::chrono::steady_clock::now();
        auto last_seen = squid->target->stack[i]->last_seen;
    
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_seen).count();
        // printf("Enemy is close to stack %d since %lld ms\n", i, diff);
    
        if (diff > 2000) {
            squid->target->stack[i]->Free = false;
            // printf("Stack %d is taken by enemy\n", i);
        }
        else if (sqrt(dx * dx + dy * dy) < 150) {
          squid->target->stack[i]->Free = false;
          // printf("Stack %d is taken by enemy\n", i);
          }
      } else {
          squid->target->stack[i]->last_seen = std::chrono::steady_clock::now(); // enemy moved away, reset timer
      }
    }
  }
}


void Path_updating(GAME* squid) {
  // Update the path of the robot
  for (int i = 0; i < 56; i++) {
    double dist_node_Sauron = sqrt(pow(squid->map->nodes[i].x*1000 - squid->Sauron->cart_pos->x, 2) + pow(squid->map->nodes[i].y*1000 - squid->Sauron->cart_pos->y, 2));
    if (dist_node_Sauron < 500) {
      squid->map->nodes[i].erased = true; // Erase the node
    }
    else {
      squid->map->nodes[i].erased = false; // Keep the node
    }
  }

  for (int i = 0; i < 59; i++) {
    if (squid->map->nodes[squid->map->edges[i].node1].erased == true || squid->map->nodes[squid->map->edges[i].node2].erased == true) {
      squid->map->edges[i].erased = true; // Erase the edge
    }
    else {
      squid->map->edges[i].erased = false; // Keep the edge
    }
  }

}

void print_free_nodes(GAME* squid) {
  // Print the free nodes
  for (int i = 0; i < 56; i++) {
    if (squid->map->nodes[i].erased == false) {
      printf("Node %d: (%f, %f)\n", i, squid->map->nodes[i].x, squid->map->nodes[i].y);
    }
  }
}