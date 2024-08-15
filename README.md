# chip-emu
Chip-8 Emulator (interpreter) implemented in C++ with SDL2.
# How to Use
Run the emulator from the command line with the filename of a Chip8 rom (.ch8) file.
```
./chipemu roms/pong.ch8
```
# How to Build
- Ensure you have installed SDL2 and SDL2_mixer. As an Arch user I got both packages with `yay`.
```
cmake .
make
```
# Design
- Based on two classes, Chip8 and Frontend
- Chip8 acts strictly as backend: Frontend tells Chip8 when to run a CPU cycle, sets key states,
and draws the contents of its screen buffer.
- Decoupled enough that it should be easy enough to write another frontend (which I might do with ncurses...)