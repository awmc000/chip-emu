#include "chip8.hpp"
#include <iostream>
#include <iomanip>

word combine(byte leftByte, byte rightByte) {
    return ((leftByte << 8) | rightByte);
}

Chip8::Chip8() {
    // Load font
    reset();    
}

void Chip8::execute(word opcode) {
    // std::cout << "EXECUTE CALLED" << std::endl;
    // std::cout << "OP: " << std::hex << opcode;
    // std::cout << std::endl;

    byte X = (opcode & 0x0F00) >> 8;    // nib 2
    byte Y = (opcode & 0x00F0) >> 4;    // nib 3
    byte N = (opcode & 0x000F);         // nib 4
    word NN = opcode & 0x00FF;          // nib 3, 4
    word NNN = opcode & 0x0FFF;         // nib 2, 3, 4

    switch (opcode & 0xF000) {
        case 0x0000:
			switch (opcode & 0x000F) {
                // Clear
                case 0x0000:
                    std::cout << "CLEAR" << std::endl;
                    opClear();
                    break;
                // Return from subroutine
                case 0x000E:
                    break;
            }
            break;
        case 0x1000:
            // Jump to NNN
            std::cout << "JUMP" << std::endl;
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
            std::cout << "SET REG" << std::endl;
            opSetRegister(X, NN);
			break;
        case 0x7000:
            std::cout << "ADD" << std::endl;
            opAdd(X, NN);
			break;
        // case 0x8000:
		// 	break;
        // case 0x9000:
		// 	break;
        case 0xA000:
            std::cout << "SET INDEX" << std::endl;
            opSetIndex(NNN);
			break;
        // case 0xB000:
		// 	break;
        // case 0xC000:
		// 	break;
        case 0xD000:
            std::cout << "DRAW" << std::endl;
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

    // Turn off VF
    variableRegisters[0xF] = 0;

    // Draw bytes I up to I+N 8px wide
    for (int y = 0; y < N; y++) {
        // Get the row of the sprite
        byte spriteRow = ram[indexRegister + y];
        
        // For bits in the byte
        // == For pixels in the row of the sprite 
        for (int x = 0; x < 8; x++) {
            bool spriteBitSet = (spriteRow >> x) & 1;
            bool bufferBitSet = (displayBuffer[y][x] & 1);


            if (spriteBitSet && bufferBitSet) {
                displayBuffer[y][x] = 0x00;
                variableRegisters[0x0F] = 1;
                std::cout << "Draw: Collision" << std::endl;
                draw = true;
            } else if (spriteBitSet) {
                displayBuffer[y][x] = 0x01;
                std::cout << "Draw: Set a display buffer bit" << std::endl;
                draw = true;
            } else {
                std::cout << "Draw: nothing happened" << std::endl;
            }

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
    
    // std::cout << "cycle()" << std::endl;

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
    
    for (int i = 0; i < CHIP8_RAM_BYTES; i++) {
        ram[i] = 0x00;
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

void Chip8::load(byte * rom)
{
    for (int i = 0; i < CHIP8_RAM_BYTES - 512; i++) {
        ram[512 + i] = rom[i];
    }
}

void Chip8::dumpState() {
    std::cout << "==== CHIP8 =====" << std::endl;
    
    std::cout << "PC: " << std::hex << (int) programCounter << std::endl;
    std::cout << "SP: " << std::hex << (int) stackPointer << std::endl;
    std::cout << "IR: " << std::hex << (int) indexRegister << std::endl;
    
    std::cout << "== REGISTERS ===" << std::endl;
    for (int i = 0; i < CHIP8_VARIABLE_REGISTERS; i++) {
        std::cout << "V" << std::hex << i 
        << ": " << std::hex << (int) variableRegisters[i] << std::endl;
    }
    std::cout << "=== TIMERS =====" << std::endl;
    std::cout << "DT: " << std::hex << (int) delayTimer << std::endl;
    std::cout << "ST: " << std::hex << (int) soundTimer << std::endl;
    
    std::cout << "===== DISPLAY ======" << std::endl;
    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            
            if (displayBuffer[y][x]) {
                std::cout << "#";
            } else {
                std::cout << "_";
            }
        }
        std::cout << "\n";
    }


    std::cout << "===== RAM ======" << std::endl;
    for (int i = 0; i < CHIP8_RAM_BYTES; i++) {
        if ( (i > 0) && (i % 4 == 0))
            std::cout << std::endl;
        std::cout << std::hex << i << ": " << std::hex << (int) ram[i] << "\t\t\t";
    }
}