# chip-emu

Emulator for the CHIP8 virtual machine (really an interpreter for
the CHIP8 programming language) implemented in C++ with SDL2.

# Setup

Install SDL2 and SDL2_mixer with your package manager of choice.

For Debian & Ubuntu Linux:
```
sudo apt upgrade
sudo apt update # Skip if your package lists are up to date
sudo apt install libsdl2-2.0-0 libsdl2-mixer-2.0-0 libsdl2-dev libsdl2-mixer-dev
```
On Arch Linux:
```
sudo pacman -Suy # Skip if your package lists are up to date
sudo pacman -S sdl2 sdl2_mixer
```

Clone this repository and navigate to the main directory.
```
gh repo clone awmc000/chip-emu
cd chip-emu
```

```
cmake .
make
```
# Usage Instructions
Run the emulator from the command line with the filename of a Chip8 rom (.ch8) file.
```
./chipemu roms/pong.ch8
```
# Controls

Press space to pause or resume emulation. The emulator starts paused when you run it.

You can also step forward through instructions using `.` while the emulator is paused.

The emulator maps the CHIP-8's 16-key hexadecimal keypad to your keyboard like this:

<img src="keypad.png" alt="drawing" width="500"/>

Controls in different games (and other programs) are decided by their authors.
