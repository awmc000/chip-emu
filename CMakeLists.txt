cmake_minimum_required(VERSION 3.29)

project(CHIP-emu VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)

add_executable(chipemu src/main.cpp src/chip8.cpp)

find_package(Catch2 3 REQUIRED)
add_executable(chiptest src/chip8.cpp test/test.cpp)
target_link_libraries(chiptest PRIVATE Catch2::Catch2WithMain)

target_link_libraries(chipemu ${SDL2_LIBRARIES})
target_link_libraries(chipemu -lSDL2_mixer)
