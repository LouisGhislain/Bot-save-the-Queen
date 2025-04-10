#include "../../../include/Robot.h"

// reset cvs->travelled_distance to 0 when the robot reaches the target

void Robot::highLevelController(int goal, std::vector<int> *path, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    // Checks if the goal has changed
    if (goal != current_destination){

        // If the goal has changed, update the current destination
        current_destination = goal;
        end_of_travel = false;

        // Find the closest node to the current position and set it as the start node
        int start_node = 1;
        double dist_to_start = INFINITY;
        double current_dist;
        for (int i = 1; i < 19; i++){
            current_dist = pow((mymap->nodes[i].x - queen->cart_pos->x),2) + pow((mymap->nodes[i].y - queen->cart_pos->y),2);
            if (current_dist < dist_to_start){
                start_node = i;
                dist_to_start = current_dist;
            }  
        }
        
        // Find the shortest path using A* algorithm
        aStar(start_node, goal, game);
        //print path size
        // fprintf(stderr, "Path size: %zu\n", (*path).size());
        current_step = 1;

    }
    
    // // If the robot has not reached the destination, call the middle level controller
    // if (current_step < (*path).size()){
        
    //     // Get the target node
    //     int target_node = (*path)[cvs->current_step];
    //     // fprintf(stderr, "Target node: %d\n", target_node);
    //     // fprintf(stderr, "Current step: %d\n", cvs->current_step);

    //     // Get the target coordinates
    //     x_coord_target = cvs->nodes[target_node].x;
    //     y_coord_target = cvs->nodes[target_node].y;

    //     // Change the target of the middle level controller
    //     //middleLevelController(x_coord_target, y_coord_target, 0, cvs);
    // }

    // // If the robot has reached the destination, stop the robot and set the end_of_travel flag to true
    // else{
    //     end_of_travel = true;
    //     // fprintf(stderr, "Reached destination\n");
    //     //print path size
    //     //fprintf(stderr, "Path size: %zu\n", (*path).size());
    // }

}
