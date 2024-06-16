#ifndef CHIP8_HPP
#define CHIP8_HPP

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_RAM_BYTES 4096
#define CHIP8_VARIABLE_REGISTERS 16
#define CHIP8_STACK_HEIGHT 16

// 16 bit type
typedef unsigned short word;

// 8 bit type
typedef unsigned char byte;

word combine(byte leftByte, byte rightByte);

class Chip8 {
private:
    // Display buffer: displayBuffer[x + (height * y)] = T / F
    byte displayBuffer[CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH];
    // Blocks/containers
    byte ram[CHIP8_RAM_BYTES];
    byte variableRegisters[CHIP8_VARIABLE_REGISTERS];
    word stack[CHIP8_STACK_HEIGHT];
    // Words
    byte stackPointer;
    word programCounter;
    word indexRegister;
    // Bytes
    byte delayTimer;
    byte soundTimer;
    // Private functions
    void execute(word);
public:
    bool draw;
    bool sound;
    byte keyState[16];
    Chip8();
    void cycle();
    void reset();
};

#endif // CHIP8_HPP