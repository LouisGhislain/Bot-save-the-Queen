#include "../../include/struct.h"

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

        squid->Sauron->cart_pos->x = mean_x;
        squid->Sauron->cart_pos->y = mean_y;
        squid->Sauron->pol_pos->angle = mean_angle;
        squid->Sauron->pol_pos->distance = mean_distance;
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

// pour le multistread il faut un void en parametre de la fonction
void fetchLidarData(void * sqid_void) {

  GAME * squid = (GAME *)sqid_void; // Cast du paramètre en GAME

  const size_t max_node_count = 8192; // Taille maximale de buffer
  sl_lidar_response_measurement_node_hq_t nodes[max_node_count];
  size_t count = max_node_count;

  sl_result result = lidardriver->grabScanDataHq(nodes, count);
  squid->queen->cart_pos->x = 170;
  squid->queen->cart_pos->y = 100;
  squid->queen->angle = M_PI/2;
  
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
      squid->map->all_map_pol[squid->map->all_map_count]->angle = angle; // Convertir en radians
      squid->map->all_map_pol[squid->map->all_map_count]->distance = distance; // Distance en mm
      squid->map->all_map_cart[squid->map->all_map_count]->x = squid->queen->cart_pos->x + distance * cos(-angle + squid->queen->angle); // Convertir en coordonnées cartésiennes
      squid->map->all_map_cart[squid->map->all_map_count]->y = squid->queen->cart_pos->y + distance * sin(-angle + squid->queen->angle); // Convertir en coordonnées cartésiennes
      
      squid->map->all_map_count++;
      // printf("Point %zu: Angle: %f, Distance: %f, X: %f, Y: %f\n", i, angle, distance, squid->map->all_map_cart[0]->x, squid->map->all_map_cart[0]->y);

    }
  }
  
 //  printf("All Map Count: %zu\n", squid->map->all_map_count);

  //Print first element of all_map_cart and all_map_pol
 //  printf("First Point: X: %f, Y: %f, Angle: %f, Distance: %f\n", squid->map->all_map_cart[0]->x, squid->map->all_map_cart[0]->y, squid->map->all_map_pol[0]->angle, squid->map->all_map_pol[0]->distance);
  
  Find_Point_Map(squid); // Vérifier si les points sont dans la carte
  if (!squid->Sauron->calibrated){
    clustering_nearest(squid); // Clustering des points
    squid->Sauron->calibrated = true; // Calibrer Sauron
  }
  else{
    clustering_calibrated(squid); // Clustering des points
  }
}

  


void Emergency_stop(GAME* squid) {
  // if Sauron is too cloose
  if (fabs(squid->Sauron->pol_pos->distance) < 500 ) {
    printf("Emergency stop\n");
    // Stop the robot
    squid->Sauron->too_close = true;
  }
}

