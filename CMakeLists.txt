cmake_minimum_required(VERSION 3.10)
 
# Set the C++ compiler explicitly for macOS
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(physics_engine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(physics_engine engine.cpp)

target_include_directories(physics_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)