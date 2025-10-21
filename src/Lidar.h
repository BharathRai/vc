#pragma once
#include <vector>
#include <cmath>
#include "AGV.h" // Forward declaration to avoid circular dependency

class AGV; // Tell the compiler that AGV is a class

class Lidar {
public:
    Lidar(double range, double fov_deg) : range_(range), fov_rad_(fov_deg* M_PI / 180.0) {}

    // Checks for obstacles within the Lidar's range and field of view.
    bool hasObstacle(AGV* self, const std::vector<AGV*>& all_agvs) {
        for (AGV* other : all_agvs) {
            if (self == other) continue; // Don't detect self

            double dx = other->x_pos - self->x_pos;
            double dy = other->y_pos - self->y_pos;
            double distance = std::sqrt(dx * dx + dy * dy);

            if (distance < range_) {
                // For simplicity, we assume the AGV is always looking along the x-axis.
                // A real implementation would use the AGV's orientation.
                if (dx > 0) { // If the other AGV is in front
                    return true;
                }
            }
        }
        return false;
    }

private:
    double range_;
    double fov_rad_;
};