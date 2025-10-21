#pragma once
#include <cmath>

class Motor {
public:
    void update(double torque_command, double load_torque) {
        if (is_faulted) {
            // If faulted, the motor produces no torque and friction stops it.
            velocity *= 0.95;
            position += velocity * dt;
            return;
        }

        double net_torque = torque_command - load_torque;
        acceleration = net_torque / inertia;
        velocity += acceleration * dt;
        position += velocity * dt;
    }

    void injectFault(bool fault_state) {
        is_faulted = fault_state;
        if (is_faulted) {
            velocity = 0;
            acceleration = 0;
        }
    }

    void setInertia(double new_inertia) { inertia = new_inertia; }
    double getPosition() const { return position; }
    double getVelocity() const { return velocity; }

private:
    const double dt = 0.001;
    double inertia = 0.5;
    double position = 0.0;
    double velocity = 0.0;
    double acceleration = 0.0;
    bool is_faulted = false;
};