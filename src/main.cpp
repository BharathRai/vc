#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "json.hpp"
#include "AGV.h" // Includes all other necessary headers

using json = nlohmann::json;

// --- Automated Test Scenarios ---

void runScenario_SimpleMove(const json& params) {
    std::cout << "--- Running Scenario: Simple Move ---" << std::endl;
    std::ofstream log_file("log_simple_move.csv");
    log_file << "Time,AGV1_X,AGV1_Setpoint\n";

    AGV agv1(1, params["agv1"]["mass"], params["agv1"]["pid"], params["lidar_config"]);
    agv1.setPosition(params["agv1"]["start_pos"]["x"], params["agv1"]["start_pos"]["y"]);

    double setpoint = params["agv1"]["setpoint"]["x"];
    std::vector<AGV*> all_agvs = { &agv1 };

    for (int i = 0; i < 5000; ++i) {
        double time = i * 0.001;
        agv1.update(setpoint, all_agvs);
        log_file << time << "," << agv1.x_pos << "," << setpoint << "\n";
    }
    log_file.close();
    std::cout << "Scenario finished. Log saved to log_simple_move.csv" << std::endl;
}

void runScenario_ObstacleAvoidance(const json& params) {
    std::cout << "\n--- Running Scenario: Obstacle Avoidance ---" << std::endl;
    std::ofstream log_file("log_obstacle.csv");
    log_file << "Time,AGV1_X,AGV2_X\n";

    AGV agv1(1, params["agv1"]["mass"], params["agv1"]["pid"], params["lidar_config"]);
    agv1.setPosition(0.0, 0.0);

    AGV agv2(2, params["agv2"]["mass"], params["agv2"]["pid"], params["lidar_config"]);
    agv2.setPosition(5.0, 0.0); // Stationary obstacle

    double agv1_setpoint = 20.0;
    std::vector<AGV*> all_agvs = { &agv1, &agv2 };

    for (int i = 0; i < 8000; ++i) {
        double time = i * 0.001;
        agv1.update(agv1_setpoint, all_agvs);
        // agv2 is stationary
        log_file << time << "," << agv1.x_pos << "," << agv2.x_pos << "\n";
    }
    log_file.close();
    std::cout << "Scenario finished. Log saved to log_obstacle.csv" << std::endl;
}


void runScenario_MotorFault(const json& params) {
    std::cout << "\n--- Running Scenario: Motor Fault Injection ---" << std::endl;
    std::ofstream log_file("log_fault.csv");
    log_file << "Time,AGV1_X,AGV1_Velocity\n";

    AGV agv1(1, params["agv1"]["mass"], params["agv1"]["pid"], params["lidar_config"]);
    agv1.setPosition(0.0, 0.0);

    double setpoint = 20.0;
    std::vector<AGV*> all_agvs = { &agv1 };

    for (int i = 0; i < 5000; ++i) {
        double time = i * 0.001;

        // Inject a fault after 2 seconds
        if (i == 2000) {
            std::cout << "Injecting motor fault at t=2.0s..." << std::endl;
            agv1.motor.injectFault(true);
        }

        agv1.update(setpoint, all_agvs);
        log_file << time << "," << agv1.x_pos << "," << agv1.motor.getVelocity() << "\n";
    }
    log_file.close();
    std::cout << "Scenario finished. Log saved to log_fault.csv" << std::endl;
}


int main() {
    std::ifstream f("../src/params.json");
    if (!f.is_open()) {
        std::cerr << "Error: Could not open params.json!" << std::endl;
        return 1;
    }
    json params = json::parse(f);

    runScenario_SimpleMove(params);
    runScenario_ObstacleAvoidance(params);
    runScenario_MotorFault(params);

    return 0;
}