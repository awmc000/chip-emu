#include "chip8.hpp"
#include <iostream>

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

    switch (opcode & 0xF000) {
        case 0x0000:
			switch (opcode & 0x000F) {
                // Clear
                case 0x0000:
                    opClear();
                    break;
                // Return from subroutine
                case 0x000E:
                    break;
            }
            break;
        case 0x1000:
            // Jump to NNN
            opJump(NNN);
			break;
        // case 0x2000:
		// 	break;
        // case 0x3000:
		// 	break;
        // case 0x4000:
		// 	break;
        // case 0x5000:
		// 	break;
        case 0x6000:
            opSetRegister(X, NN);
			break;
        case 0x7000:
            opAdd(X, NN);
			break;
        // case 0x8000:
		// 	break;
        // case 0x9000:
		// 	break;
        case 0xA000:
            opSetIndex(NNN);
			break;
        // case 0xB000:
		// 	break;
        // case 0xC000:
		// 	break;
        case 0xD000:
            opDraw(X, Y, N);
			break;
        // case 0xE000:
		// 	break;
        // case 0xF000:
		// 	break;
        default:
            std::cerr << "Unsupported instruction" << opcode << std::endl;
            break;
    }
}

void Chip8::opClear() {
    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            displayBuffer[y][x] = 0x00;
        }
    }
}

void Chip8::opJump(word NNN) {
    programCounter = NNN;
}

void Chip8::opSetRegister(byte X, word NN)
{
    variableRegisters[X] = NN;
}

void Chip8::opAdd(byte X, word NN)
{
    variableRegisters[X] += NN;
}

void Chip8::opSetIndex(word NNN)
{
    indexRegister = NNN;
}

void Chip8::opDraw(byte X, byte Y, byte N)
{
    byte xCoord = variableRegisters[X] % CHIP8_SCREEN_WIDTH;
    byte yCoord = variableRegisters[Y] % CHIP8_SCREEN_HEIGHT;

    // Turn off draw flag
    draw = false;

    // Draw bytes I up to I+N 8px wide
    for (int y = 0; y < N; y++) {
        // Get the row of the sprite
        byte spriteRow = ram[indexRegister + y];
        
        // For bits in the byte-row
        for (int x = 0; x < 8; x++) {
    
            if (displayBuffer[yCoord][xCoord] != ((spriteRow >> x) & 1))
                draw = true;
            
            displayBuffer[yCoord][xCoord] ^= ((spriteRow >> x) & 1);
    
            xCoord++;
    
            if (xCoord == CHIP8_SCREEN_WIDTH) {
                break;
            }
        }
        xCoord -= 7;
        yCoord++;

        if (yCoord == CHIP8_SCREEN_HEIGHT) {
            break;
        }
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
        if (soundTimer == 1) {
            // Set sound flag
            sound = true;
        }
        soundTimer--;
    }
}

void Chip8::reset() {
    // Clear display buffer
    opClear();
    
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