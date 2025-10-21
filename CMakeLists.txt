# Specifies the minimum version of CMake required.
cmake_minimum_required(VERSION 3.10)

# Defines the project name.
project(VirtualCommissioningTier3)

# Sets the C++ standard to C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tells CMake to look for header files (.h) in the 'src' directory.
include_directories(src)

# Creates the final executable named 'simulation' from the main.cpp source file.
add_executable(simulation src/main.cpp)