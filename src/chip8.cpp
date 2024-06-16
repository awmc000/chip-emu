#include "chip8.hpp"

word combine(byte leftByte, byte rightByte) {
    return ((leftByte << 8) | rightByte);
}

Chip8::Chip8() {
    // Load font
    reset();    
}

void Chip8::execute(word opcode) {

    byte X = (opcode & 0x0F00) >> 8;    // nib 2
    byte Y = (opcode & 0x00F0) >> 4;    // nib 3
    byte N = X;                         // nib 2
    word NN = opcode & 0x00FF;          // nib 3, 4
    word NNN = opcode & 0x0FFF;         // nib 2, 3, 4

    switch (0xF000 & opcode) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
        case 0x8000:
        case 0x9000:
        case 0xA000:
        case 0xB000:
        case 0xC000:
        case 0xD000:
        case 0xE000:
        case 0xF000:
    }
}

void Chip8::cycle() {
    // Fetch
    word opcode = combine(ram[programCounter], ram[programCounter + 1]);
    programCounter += 2;
    // Decode, Execute
    execute(opcode);
    // Update timers
    if (delayTimer > 0) {
        delayTimer--;
    }
    if (soundTimer > 0) {
        soundTimer--;
        if (soundTimer == 0) {
            // Set sound flag
            sound = true;
        }
    }
}

void Chip8::reset() {
    // Clear display buffer
    for (int i = 0 ; i < CHIP8_SCREEN_HEIGHT*CHIP8_SCREEN_WIDTH; i++) {
        displayBuffer[i] = 0x00;
    }
    
    // Clear registers
    for (int i = 0; i < CHIP8_VARIABLE_REGISTERS; i++) {
        variableRegisters[i] = 0x0000;
    }
    
    // Clear stack
    for (int i = 0; i < CHIP8_STACK_HEIGHT; i++) {
        stack[i] = 0x0000;
    }

    // Clear SP, PC, I, DT, ST 
    stackPointer    = 0x00;
    programCounter  = 0x0000;
    indexRegister   = 0x0000;
    delayTimer      = 0x00;
    soundTimer      = 0x00;

    // Set PC to start
    programCounter = 0x200;
}