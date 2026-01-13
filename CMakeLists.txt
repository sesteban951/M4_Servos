# cmake version to use
cmake_minimum_required(VERSION 3.12)

# project name
project(m4_servos)

# set flags
set(CMAKE_CXX_STANDARD 17)  # Need c++ 17 for inline variables
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3 -pthread")

# add executables
add_executable(simple src/simple.cpp)
add_executable(comms_example src/example.cpp src/servo_comm.cpp)