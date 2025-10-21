#pragma once
#include <string>
#include <vector>
#include "Motor.h"
#include "PIDController.h"
#include "Lidar.h"

class AGV {
public:
    int id;
    double x_pos = 0.0, y_pos = 0.0; // We'll simplify to 1D motion for now.
    double mass;
    bool emergency_stop = false;

    // Components of the AGV
    Motor motor;
    PIDController pid;
    Lidar lidar;

    AGV(int agv_id, double m, const json& pid_params, const json& lidar_params)
        : id(agv_id), mass(m),
        pid(pid_params["Kp"], pid_params["Ki"], pid_params["Kd"]),
        lidar(lidar_params["range"], lidar_params["field_of_view_deg"]) {
        // Inertia is simplified based on mass
        motor.setInertia(0.1 * mass);
    }

    void setPosition(double x, double y) {
        x_pos = x;
        y_pos = y;
        // This is a simplification; a real system would need to update the motor's internal position.
    }

    void update(double setpoint, const std::vector<AGV*>& all_agvs) {
        if (emergency_stop || lidar.hasObstacle(this, all_agvs)) {
            motor.update(0, 0); // Stop motor
        }
        else {
            double current_pos = motor.getPosition();
            double control_signal = pid.calculate(setpoint, current_pos);
            motor.update(control_signal, 0.0);
        }
        // Update the AGV's world position based on its motor's internal position.
        // This is simplified to 1D motion.
        x_pos = motor.getPosition();
    }
};