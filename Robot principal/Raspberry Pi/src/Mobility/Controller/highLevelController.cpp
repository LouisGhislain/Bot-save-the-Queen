#include "../../../include/Robot.h"

// reset cvs->travelled_distance to 0 when the robot reaches the target

void Robot::highLevelController(int goal, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    fprintf(stderr, "High level execution\n");

    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        // By default, in the high level controller, the movement type is deplacement
        GLOBAL_params = deplacement;
    }

    // Checks if the goal has changed
    if (goal != current_destination){

        fprintf(stderr, "New goal: %d\n", goal);

        // If the goal has changed, update the current destination
        current_destination = goal;
        end_of_travel = false;
        {
            std::lock_guard<std::mutex> lock(flags);
            last_step = false;
        }
        

        // Find the closest node to the current position and set it as the start node
        int start_node;
        double dist_to_start = INFINITY;
        double current_dist;

        double my_x, my_y;
        {
            std::lock_guard<std::mutex> lock(queen->position_mutex);
            my_x = queen->cart_pos->x;
            my_y = queen->cart_pos->y;
        }

        for (int i = 0; i < 19; i++){
            current_dist = pow((mymap->nodes[i].x - my_x),2) + pow((mymap->nodes[i].y - my_y),2);
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
    double myrho;
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        myrho = GLOBAL_rho;
    }
    
    // If the robot has reached the target node, update the current step
    if((myrho < d1_change_target) && (current_step < path.size())){
        current_step++;
    }

    // If the robot has not reached the destination, call the middle level controller
    if (current_step < path.size()){

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
    else{
        {
            std::lock_guard<std::mutex> lock(flags);
            end_of_travel = true;
            fprintf(stderr, "End of travel\n");
        }
    }

}
