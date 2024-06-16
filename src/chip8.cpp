#include "chip8.hpp"

word combine(byte leftByte, byte rightByte) {
    return ((leftByte << 8) | rightByte);
}

Chip8::Chip8() {
    // Load font
    reset();    
}

void Chip8::cycle() {
    // Fetch
    word opcode = combine(ram[programCounter], ram[programCounter + 1]);
    programCounter += 2;
    // Decode, Execute
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