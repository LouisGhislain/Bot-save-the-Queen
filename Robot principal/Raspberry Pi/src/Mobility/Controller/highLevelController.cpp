#include "../../../include/Robot.h"

// reset cvs->travelled_distance to 0 when the robot reaches the target

void Robot::highLevelController(int goal, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    fprintf(stderr, "High level execution\n");

    // By default, in the high level controller, the movement type is deplacement
    params = deplacement;

    // Checks if the goal has changed
    if (goal != current_destination){

        fprintf(stderr, "New goal: %d\n", goal);

        // If the goal has changed, update the current destination
        current_destination = goal;
        end_of_travel = false;
        last_step = false;

        // Find the closest node to the current position and set it as the start node
        int start_node;
        double dist_to_start = INFINITY;
        double current_dist;
        for (int i = 0; i < 19; i++){
            current_dist = pow((mymap->nodes[i].x - queen->cart_pos->x),2) + pow((mymap->nodes[i].y - queen->cart_pos->y),2);
            if (current_dist < dist_to_start){
                start_node = i;
                dist_to_start = current_dist;
            }  
        }
        
        fprintf(stderr, " start and goal nodes: %d %d\n", start_node, goal);
        // Find the shortest path using A* algorithm
        aStar(start_node, goal, game);

        // Aim for the second node in the path, the first beeing where we are (the closest node)
        current_step = 0;

    }

    fprintf(stderr, "path size : %d \n", path.size());
    printPath();
    fprintf(stderr, "rho : %f \n", rho);
    //rho = sqrt(pow((x_coord_target - queen->cart_pos->x),2) + pow((y_coord_target - queen->cart_pos->y),2)); // Quentin veut trouver plus élégant
    // If the robot has reached the target node, update the current step
    if(rho < d1_change_target){
        current_step++;
        fprintf(stderr, "Current step: %d\n", current_step);
    }

    // If the robot has not reached the destination, call the middle level controller
    if (current_step < path.size()){

        // If last step of the travel 
        if (current_step == path.size()-1){
            last_step = true;
        }
        
        // Get the target node
        int target_node = path[current_step];

        // Get the target coordinates
        // And change the target of the middle level controller
        x_coord_target = mymap->nodes[target_node].x;
        y_coord_target = mymap->nodes[target_node].y;

        fprintf(stderr, "Target node: %d\n", target_node);
    }

    // If the robot has reached the destination, stop the robot and set the end_of_travel flag to true
    else{
        end_of_travel = true;
        fprintf(stderr, "End of travel\n");
    }

}
