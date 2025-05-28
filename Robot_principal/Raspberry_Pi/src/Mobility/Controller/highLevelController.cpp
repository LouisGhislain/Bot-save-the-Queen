#include "../../../include/Robot.h"

// reset cvs->travelled_distance to 0 when the robot reaches the target

void Robot::highLevelController(int goal, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    //fprintf(stderr, "High level execution\n");

    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        // By default, in the high level controller, the movement type is deplacement
        GLOBAL_params = deplacement;
    }
    double my_distl, my_distr;
    {
        std::lock_guard<std::mutex> lock(dist_mutex);
        my_distl = distl;
        my_distr = distr;
    }

    double my_x, my_y;
    {
        std::lock_guard<std::mutex> lock(queen->position_mutex);
        my_x = queen->cart_pos->x;
        my_y = queen->cart_pos->y;
    }

    // Checks if the goal has changed
    if (goal != current_destination){

        // If the goal has changed, update the current destination
        current_destination = goal;
        end_of_travel = false;

        travelled_distance = 0;

        last_distl_high = my_distl;
        last_distr_high = my_distr;

        {
            std::lock_guard<std::mutex> lock(flags);
            last_step = false;
        }

        // Find the closest node to the current position and set it as the start node
        int start_node;
        double dist_to_start = INFINITY;
        double current_dist;

        for (int i = 0; i < 100; i++){
            current_dist = pow((mymap->nodes[i].x - my_x),2) + pow((mymap->nodes[i].y - my_y),2);
            if (current_dist < dist_to_start){
                start_node = i;
                dist_to_start = current_dist;
            }  
        }
        
        // Find the shortest path using A* algorithm
        aStar(start_node, goal, game);
        printPath();

        // Aim for the second node in the path, the first beeing where we are (the closest node)
        current_step = 1;

    }

    //fprintf(stderr, "path size : %d \n", path.size());
    //printPath();
    double myrho = sqrt(pow((mymap->nodes[path[current_step]].x - my_x),2) + pow((mymap->nodes[path[current_step]].y - my_y),2));

    // // Print the node it is aiming for :
    // fprintf(stderr, "Aiming for node: %d\n", path[current_step]);

    // // Print the path size
    // fprintf(stderr, "Path size: %d\n", path.size());

    // // Print the path with the printpath function
    // printPath();

    // // Print end_of_travel
    // fprintf(stderr, "End of travel: %d\n", end_of_travel);

    
    // // Print myrho
    // fprintf(stderr, "myrho: %f\n", myrho);

    // // Print stop_robot_distance
    // fprintf(stderr, "stop_robot_distance: %f\n", GLOBAL_params.stop_robot_distance);

    // If the robot has reached the target node, update the current step
    if((myrho < d1_change_target) && (current_step < path.size()-1)){
        current_step++;
        // Print step ++
        //fprintf(stderr, "Step ++");
    }

    
    // COMPUTING THE TRAVELLED DISTANCE
    {
        std::lock_guard<std::mutex> lock(travelled_distance_mutex);
        travelled_distance += abs(((my_distl + my_distr)-(last_distl_high + last_distr_high))/2);
    }
    
    // COMPUTING THE RHO TO GOAL
    {
        std::lock_guard<std::mutex> lock(rho_to_goal_mutex);
        rho_to_goal = sqrt(pow((mymap->nodes[goal].x - my_x),2) + pow((mymap->nodes[goal].y - my_y),2));
    }

    // If the robot has not reached the destination, call the middle level controller
    if (current_step < path.size()){
        // Print step
        //fprintf(stderr, "Step: %d\n", current_step);

        // If last step of the travel 
        if (current_step == path.size()-1){
            {
                std::lock_guard<std::mutex> lock(flags);
                last_step = true;
            }
        }
        
        // Get the target node
        int target_node = path[current_step];

        {
            // Get the target coordinates
            // And change the target of the middle level controller
            std::lock_guard<std::mutex> lock(coord_mutex);
            GLOBAL_x_coord_target = mymap->nodes[target_node].x;
            GLOBAL_y_coord_target = mymap->nodes[target_node].y;
        }

    }
    // If the robot has reached the destination, stop the robot and set the end_of_travel flag to true
    if(rho_to_goal < GLOBAL_params.stop_robot_distance) {
        {
            std::lock_guard<std::mutex> lock(flags);
            end_of_travel = true;
        }
        fprintf(stderr, "End of travel\n");
        travelled_distance = 0;
    }

}
