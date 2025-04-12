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

#include "../include/Robot.h"
#include "../include/struct.h"
#include "../include/lidar.h"

// Variables globales pour le multithreading
std::atomic<bool> running{true};

Robot robot;

// Function to handle SIGINT (Ctrl+C)
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here  
    // terminate program

    running.store(false);
    robot.stop();

    exit(signum);
}   

// Controller thread function that runs at specified interval
void loop_1ms(GAME *game){
    using namespace std::chrono;
    
    std::cout << "1ms loop started" << std::endl;
    
    while (running) {
        auto start_time = steady_clock::now();
        // functions to be executed at the same iteration
        robot.updateOdometry(game);
        robot.lowLevelController();

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

    while (running) {
        auto start_time = steady_clock::now();
        // functions to be executed at the same iteration

        robot.middleLevelController(game);

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
    
    while (running) {
        auto start_time = steady_clock::now();
        
        // functions to be executed at the same iteration
        //FSM();

    // Print robot state
    std::cout << "Robot state: " << robot.STATE << std::endl;


    switch (robot.STATE)
    {
    /*
    case GRABBING : 
        robot.teensy_send_command(0x02); // Grab
        robot.STATE = WAITING ; 
        break ; 
    case WAITING : 
        usleep(3000000);
        robot.STATE = SEPARATING ;
        break ;
    case SEPARATING : 
        robot.teensy_build(game);
        robot.STATE = STOPPED ; 
        break; 
    */
    case MOVING_FIRST_STACK:
        robot.highLevelController(0, game);
        if (robot.end_of_travel){
            robot.STATE = FIRST_MANEUVER;
        }
        break;

    case FIRST_MANEUVER:
        robot.maneuver(0.10, game);
        robot.STATE = GRABBING;
        break;

    case GRABBING:
        std::cout << "End of manoeuvre: " << robot.end_of_manoeuvre << std::endl;
        if (robot.end_of_manoeuvre){
            robot.teensy_send_command(0x02); // Grab
            robot.STATE = STOPPED;
        }
        break;

    
    case STOPPED:
        usleep(30000);
        break;

    default:
        break;
    }
        // Calculate how long to sleep to maintain desired frequency
        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
        auto sleep_time = milliseconds(100) - elapsed;
        
        if (sleep_time > milliseconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            // Controller took longer than interval - log warning
            std::cout << "Warning: 100ms loop exceeded time budget by "
                      << -sleep_time.count() << "ms" << std::endl;
        }
    }
}



int main() {

    // Signal handler initialization
    signal(SIGINT, signalHandler);

    GAME *game = init_game();
    
    running.store(true);

    //Screen screen;

    robot.loadNodes("src/Mobility/Localization/nodes.txt", game);
    robot.loadEdges("src/Mobility/Localization/links.txt", game);   

    // Specify the starting position of the robot
    // 0 = blue_bottom, 1 = blue_side, 2 = yellow_bottom, 3 = yellow_side
    robot.starting_pos = 2;
    
    try {
        robot.start();  // This will initialize SPI and perform other setup tasks.
        robot.initCoords(game); // Initialize coordinates
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    // Create controller threads with different frequencies
    // truc de margoulin pour appeler low level controller via fonction lambda
    std::thread thread_1ms(loop_1ms, game);
    std::thread thread_10ms(loop_10ms, game);
    std::thread thread_100ms(loop_100ms, game);


    // Let the system run for a specified time (e.g., 10 seconds)
    std::cout << "Controllers running. Press Enter to stop..." << std::endl;
    std::cin.get();

    running.store(false);
    // Arrêter les threads et nettoyer les ressources
    thread_1ms.join();
    thread_10ms.join();
    thread_100ms.join();

    free_game(game);
    
    std::cout << "Programme terminé avec succès" << std::endl;

    return 0;
}