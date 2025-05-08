#include <iostream>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <limits>
#include <stdio.h>
#include <mutex>
#include <atomic>
#include <chrono>
#include <csignal>
#include <condition_variable>

#include <sys/select.h>
#include <unistd.h>

#include "../include/Robot.h"
#include "../include/struct.h"
#include "../include/lidar.h"
#include "../include/FSM.h"

// // Variables globales pour le multithreading
// std::atomic<bool> running{true};

Robot *robot = new Robot();

// Function to handle SIGINT (Ctrl+C)
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here  
    // terminate program

    //running.store(false);
    robot->running = false; // Set the running flag to false
    robot->stop();

    usleep(0.1*1000000); // Wait for 1 second to ensure the robot stops

    robot->stop();
    
    //exit(signum);
}   

// Controller thread function that runs at specified interval
void loop_1ms(GAME *game){
    using namespace std::chrono;
    
    std::cout << "1ms loop started" << std::endl;
    
    while (robot->running) {
        auto start_time = steady_clock::now();
        
        //===========================================================================
        // 1ms loop - START
        //===========================================================================

        robot->updateOdometry(game);
        robot->lowLevelController();
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));

        //===========================================================================
        // 1ms loop - END
        //===========================================================================

        // Calculate how long to sleep to maintain desired frequency
        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
        auto sleep_time = milliseconds(1) - elapsed;
        
        if (sleep_time > milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            // Controller took longer than interval - log warning
            std::cout << "Warning: 1ms loop exceeded time budget by "
                      << -sleep_time.count() << "ms" << std::endl;
        }
    }
}

void loop_10ms(GAME *game){
    using namespace std::chrono;
    
    std::cout << "10ms loop started" << std::endl;

    while (robot->running) {
        auto start_time = steady_clock::now();
        //===========================================================================
        // 10ms loop - START
        //===========================================================================
        auto [distance_ennemy, angle_ennemy] = robot->get_distance_to_ennemy(game); 

        if (robot->backwards == true){ // if the robot goes backward, we change the interpreted angle
            angle_ennemy = 180 - angle_ennemy; // get the absolute value of the angle
        }
        // profil d'évitement d'adversaire (voir screenshot desmos)
        if (robot->avoidance_loop_activated || (angle_ennemy < STOP_ANGLE_ENNEMY && distance_ennemy*(pow(angle_ennemy,2) * coef_detection_profile / pow(STOP_ANGLE_ENNEMY,2) + 1) < STOP_DISTANCE_ENNEMY_FRONT)) {  // stop distance parameter in the lidar.h file
            //robot->reaction_to_ennemy_smart(game); // uncomment this line to use the smart reaction
            robot->stop_if_ennemy(); // stop the robot
            //digitalWrite(BUZZER_PIN, HIGH);    // activate the buzzer
            //fprintf(stderr, "ennemy detected\n");
        }
        else{
            robot->avoidance_loop_activated = false; // reset the ennemy avoidance case (because when we don't detect the ennemy anymore we cant update this flag in the ennemy detection loop)
            robot->CASE_ennemy_avoidance = 0; // reset the ennemy avoidance case (because when we don't detect the ennemy anymore we cant update this flag in the ennemy detection loop)
            robot->middleLevelController(game);
            //digitalWrite(BUZZER_PIN, LOW); // deactivate the buzzer
        }

        //============================================================================
        // 10ms loop - END
        //============================================================================

        // Calculate how long to sleep to maintain desired frequency
        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
        auto sleep_time = milliseconds(10) - elapsed;
        
        if (sleep_time > milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            // Controller took longer than interval - log warning
            std::cout << "Warning: 10ms loop exceeded time budget by "
                      << -sleep_time.count() << "ms" << std::endl;
        }
    }
}

void loop_100ms(GAME *game){
    using namespace std::chrono;
    
    std::cout << "100ms loop started" << std::endl;
    
    while (robot->running) {
        auto start_time = steady_clock::now();
        
        //===========================================================================
        // 100ms loop - START
        //===========================================================================

        choose_start(robot, game);
        // print_match_time(game);

        //===========================================================================
        // 100ms loop - END
        //===========================================================================

        // Calculate how long to sleep to maintain desired frequency
        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
        auto sleep_time = milliseconds(20) - elapsed;
        
        if (sleep_time > milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
        // else {
            // Controller took longer than interval - log warning
            // std::cout << "Warning: 100ms loop exceeded time budget by "
            //           << -sleep_time.count() << "ms" << std::endl;
        // }
    }
}

// Fonction thread pour la récupération des données LIDAR
void lidar_thread_func(void* game_void) {
    using namespace std::chrono;
    GAME* game = (GAME*)game_void;
    
    
    std::cout << "lidar thread started" << std::endl;

    while (robot->running) {
        auto start_time = steady_clock::now();
        // Appeler la fonction de récupération des données LIDAR

        //===========================================================================
        // lidar loop - START - (undifined period)
        //===========================================================================

        fetchLidarData(game);
        // print_Sauron_position(game);

        //===========================================================================
        // lidar loop - END - (undifined period)
        //===========================================================================
        
        // Calculate how long to sleep to maintain desired frequency
        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
        //fprintf(stderr, "computation time lidar = %d\n", elapsed);

        // Court délai pour éviter une utilisation excessive du CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 10ms de délai
    }
}



int main() {

    // Signal handler initialization
    signal(SIGINT, signalHandler);

    GAME *game = init_game();

    // // Print queen's coordinates and angle every 10ms
    // while (running) {
    //     std::cout << "Queen's position and angle : (" << game->queen->cart_pos->x << ", " << game->queen->cart_pos->y << ") Angle : " << game->queen->angle << std::endl;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // }

    pos_of_stack(game);
    
    //running.store(true);
    robot->running = true; // Set the running flag to true

    robot->loadNodes("src/Mobility/Localization/nodes.txt", game);
    robot->loadEdges("src/Mobility/Localization/links.txt", game);   

    // Specify the starting position of the robot

    // 0=BLUE=0
    //                                                                            (1 = BLUE SIDE)

    // 2=YELLOW=2
    //                                                                            (3 = YELLOW SIDE)

    //robot->starting_pos = 0; // 0 or 2

    int inputStarting_pos = -1;
    std::string line;
    std::cout << "Enter starting position (BLUE=0     YELLOW=2) : ";
    while (robot->running) {
        std::fflush(stdout);

        // wait up to 0.5s for keystroke
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        struct timeval tv{0, 500000};  // 0.5 seconds

        int ret = select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv);
        if (ret < 0) {
            if (errno == EINTR || !robot->running) break; // interrupted!
            perror("select");
            break;
        }
        if (ret == 0) {
            continue;  // timeout, loop around and re-check robot->running
        }

        // there's data, read the whole line
        if (!std::getline(std::cin, line)) {
            // EOF or error (e.g. Ctrl+D), treat as interrupt
            break;
        }

        // try to parse it
        try {
            inputStarting_pos = std::stoi(line);
        } catch (...) {
            std::cout << "Invalid input. Please enter a number (0 or 2)." << std::endl;
            continue;
        }

        if (inputStarting_pos == 0 || inputStarting_pos == 2) {
            break;  // valid!
        }
        std::cout << "Invalid input. Please enter 0 or 2." << std::endl;
    }

    if (!robot->running) {
        // handle early exit if you want, e.g. return or throw
    }

    robot->starting_pos = inputStarting_pos;

    // std::cout << "Enter starting position (BLUE=0     YELLOW=2) :\n";
    // std::cin >> robot->starting_pos;
    
    try {
        robot->start();  // This will initialize SPI and perform other setup tasks.
        robot->initCoords(game); // Initialize coordinates
        init_connectLidar(); // Initialiser et démarrer le LIDAR
        robot->screen_clear();
       // robot->screen_display_intro();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    robot->screen_menu(game);
    // must be the last thing to do before starting the game
    robot->wait_starting_cord(game); // Wait for the starting cord to be inserted
    
    // Create controller threads with different frequencies
    std::thread thread_1ms(loop_1ms, game);
    std::thread thread_10ms(loop_10ms, game);
    std::thread thread_100ms(loop_100ms, game);
    std::thread thread_lidar(lidar_thread_func, game);

    // Waiting for interrupt signal:
    while (robot->running) {
        pause();  // wait for any signal (SIGINT wakes it up)
    }

    // Arrêter les threads et nettoyer les ressources
    thread_1ms.join();
    thread_10ms.join();
    thread_100ms.join();
    thread_lidar.join();

    free_game(game);
    
    std::cout << "Programme terminé avec succès" << std::endl;
    return 0;
}