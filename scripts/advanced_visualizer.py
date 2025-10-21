import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

def plot_simple_move(filepath):
    df = pd.read_csv(filepath)
    plt.figure(figsize=(12, 6))
    plt.plot(df['Time'], df['AGV1_X'], label='AGV1 Position')
    plt.axhline(y=df['AGV1_Setpoint'].iloc[-1], color='r', linestyle='--', label='Setpoint')
    plt.title('Scenario: Simple A-to-B Move')
    plt.xlabel('Time (s)')
    plt.ylabel('Position (m)')
    plt.legend()
    plt.grid(True)
    plt.show()

def plot_obstacle_avoidance(filepath):
    df = pd.read_csv(filepath)
    plt.figure(figsize=(12, 6))
    plt.plot(df['Time'], df['AGV1_X'], label='AGV1 (Moving)')
    plt.plot(df['Time'], df['AGV2_X'], label='AGV2 (Obstacle)', linestyle=':')
    plt.title('Scenario: Obstacle Avoidance (Lidar-based Stop)')
    plt.xlabel('Time (s)')
    plt.ylabel('Position (m)')
    plt.legend()
    plt.grid(True)
    plt.show()

def plot_fault_injection(filepath):
    df = pd.read_csv(filepath)
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 9), sharex=True)
    fig.suptitle('Scenario: Motor Fault Injection at t=2.0s', fontsize=16)
    
    ax1.plot(df['Time'], df['AGV1_X'], label='AGV1 Position')
    ax1.set_ylabel('Position (m)')
    ax1.grid(True)
    ax1.legend()

    ax2.plot(df['Time'], df['AGV1_Velocity'], label='AGV1 Velocity', color='orange')
    ax2.set_ylabel('Velocity (m/s)')
    ax2.set_xlabel('Time (s)')
    ax2.grid(True)
    ax2.legend()
    plt.show()

def main():
    # Set up command-line argument parsing
    parser = argparse.ArgumentParser(description="Visualize results from the virtual commissioning simulation.")
    parser.add_argument('scenario', type=str, choices=['simple', 'obstacle', 'fault'],
                        help="The test scenario to plot ('simple', 'obstacle', 'fault').")
    
    args = parser.parse_args()

    # Map scenario argument to filename and plot function
    scenario_map = {
        'simple': ('log_simple_move.csv', plot_simple_move),
        'obstacle': ('log_obstacle.csv', plot_obstacle_avoidance),
        'fault': ('log_fault.csv', plot_fault_injection)
    }

    log_filename, plot_function = scenario_map[args.scenario]
    
    # Path is relative to the project root, where this script should be run from
    filepath = os.path.join('build', log_filename)

    if not os.path.exists(filepath):
        print(f"Error: Log file not found at '{filepath}'")
        print("Please build and run the C++ simulation first.")
        return

    plot_function(filepath)


if __name__ == "__main__":
    main()