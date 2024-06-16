cmake_minimum_required(VERSION 3.29)

project(CHIP-emu VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)

add_executable(chipemu src/main.cpp src/chip8.cpp)
target_link_libraries(chipemu ${SDL2_LIBRARIES})
