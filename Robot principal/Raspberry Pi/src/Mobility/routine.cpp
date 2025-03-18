#include "../../include/Robot.h"

/**
 * @brief Routine to pass through predefined points
 * 
 * This function makes the robot move through points using the middleLevelController.
 */
void Robot::routine() {
    // Define 3 target points (x, y, angle in degrees)
    std::vector<std::tuple<double, double, double>> points = {
        {100.0, -100.0, 0.0},   // Point 1: 100 cm forward, angle 0 degrees
        {200.0, 0, 0.0}, // Point 2: 100 cm to the right, angle 90 degrees
        {200.0, -120.0, 0.0}   // Point 3: 100 cm backward, angle 180 degrees
    };

    // Loop through each point and move to it
    for (const auto& point : points) {
        double x = std::get<0>(point);
        double y = std::get<1>(point);
        double angle = std::get<2>(point);

        std::cout << "Moving to point: (" << x << ", " << y << ") with angle: " << angle << " degrees" << std::endl;
        middleLevelController(x, y, angle);
    }

    std::cout << "Routine completed!" << std::endl;
}