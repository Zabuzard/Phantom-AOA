cmake_minimum_required(VERSION 3.23)
project(Phantom_AOA)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Phantom_AOA main.cpp engine/Simulation.cpp engine/Simulation.h engine/Entity.h phantom/TestEntity.h)
