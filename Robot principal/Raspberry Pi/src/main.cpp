#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <limits>
#include <stdio.h>
#include "../include/Robot.h"
#include "../include/struct.h"
#include "../include/lidar.h"

// // Variables globales pour le multithreading
// volatile bool running = true;
// pthread_mutex_t data_mutex;
// pthread_t lidar_thread;

// // Shared resource
// int shared_counter = 0;

// void* worker_function(void* arg) {
//     int thread_id = *(int*)arg;
    
//     // Simulate some work
//     for (int i = 0; i < 5; i++) {
//         // Lock the mutex before accessing shared resource
//         pthread_mutex_lock(&mutex);
        
//         // Critical section
//         shared_counter++;
//         printf("Thread %d: counter = %d\n", thread_id, shared_counter);
        
//         // Unlock the mutex
//         pthread_mutex_unlock(&mutex);
        
//         // Sleep to simulate work (not necessary in real applications)
//         usleep(500000); // 500ms
//     }
    
//     printf("Thread %d completed\n", thread_id);
//     return NULL;
// }

int main() {

    // // Initialiser le mutex
    // if (pthread_mutex_init(&data_mutex, NULL) != 0) {
    //     std::cerr << "Échec de l'initialisation du mutex" << std::endl;
    //     return 1;
    // }

    Robot robot;

    GAME *game = init_game(); 
    //Screen screen;

    robot.loadNodes("src/Mobility/Localization/nodes.txt", game);
    robot.loadEdges("src/Mobility/Localization/links.txt", game);
    char choice;

    std::cout << "Select an option:" << std::endl;
    std::cout << "  a: boucles threads" << std::endl;
    std::cout << "  b: main thread" << std::endl;
    std::cout << "  c: path planning test" << std::endl;
    std::cout << "  d: N/A" << std::endl;
    std::cout << "  e: N/A" << std::endl;
    std::cout << "  f: N/A" << std::endl;
    std::cout << "  g: N/A" << std::endl;
    std::cout << "  h: N/A" << std::endl;
    std::cout << "  i: N/A" << std::endl;
    std::cout << "  j: N/A" << std::endl;
    std::cout << "  k: N/A" << std::endl;
    std::cout << "  l: N/A" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
        robot.start();  // This will initialize SPI and perform other setup tasks.
        robot.initCoords(game); // Initialize coordinates
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    switch(choice) {
        case 'a': {
            // std::cout << "Starting threads..." << std::endl;
            // // Start the threads here
            // const int num_threads = 4;
            // pthread_t threads[num_threads];
            // int thread_ids[num_threads];
            
            // printf("Starting threads on Raspberry Pi\n");
            
            // // Create threads
            // for (int i = 0; i < num_threads; i++) {
            //     thread_ids[i] = i;
            //     pthread_create(&threads[i], NULL, worker_function, &thread_ids[i]);
            //     printf("Created thread %d\n", i);
            // }
            
            // // Join threads
            // for (int i = 0; i < num_threads; i++) {
            //     pthread_join(threads[i], NULL);
            // }
            
            // // Destroy the mutex
            // pthread_mutex_destroy(&mutex);
            
            // printf("All threads completed. Final counter value: %d\n", shared_counter);
            // return 0;


            break;
        }

        case 'b': {
            std::cout << "Running main thread..." << std::endl;
            // Run the main thread logic here
            break;
        }
        
        case 'c': {
            robot.aStar(1, 19, game);
            robot.printPath();
            break;
        }

        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }

    // Arrêter le thread et nettoyer les ressources
    // running = false;
    // pthread_join(lidar_thread, NULL);
    //pthread_mutex_destroy(&data_mutex);
    //free_game(game);
    
    std::cout << "Programme terminé avec succès" << std::endl;

    return 0;
}