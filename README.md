# Project: Virtual Commissioning Platform

A simplified prototype of a virtual commissioning and parameter tuning platform for AGVs.

## How to Build and Run on Windows

1.  **Configure**: `mkdir build && cd build && cmake ..`
2.  **Compile**: `cmake --build .`
3.  **Run Simulation**: `.\Debug\simulation.exe`
4.  **Visualize**: `python ..\scripts\visualize.py`

Edit `src/params.json` to tune the controller and re-run the simulation to see the effect.