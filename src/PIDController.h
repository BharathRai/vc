#pragma once

class PIDController {
public:
    double Kp = 0.0, Ki = 0.0, Kd = 0.0;

    PIDController(double p, double i, double d) : Kp(p), Ki(i), Kd(d) {}

    double calculate(double setpoint, double process_variable) {
        double error = setpoint - process_variable;
        integral += error * dt;
        double derivative = (error - previous_error) / dt;
        previous_error = error;
        return Kp * error + Ki * integral + Kd * derivative;
    }

    void reset() {
        previous_error = 0.0;
        integral = 0.0;
    }

private:
    const double dt = 0.001;
    double previous_error = 0.0;
    double integral = 0.0;
};