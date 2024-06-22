#/bin/bash
g++ -I "include/" -pg -g src/chip8.cpp src/main.cpp -lSDL2 -o debug
gdb debug