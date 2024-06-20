#ifndef CHIP8_HPP
#define CHIP8_HPP

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_RAM_BYTES 4096
#define CHIP8_VARIABLE_REGISTERS 16
#define CHIP8_STACK_HEIGHT 16
#define CHIP8_ROM_BYTES 3584

// 16 bit type
typedef unsigned short word;

// 8 bit type
typedef unsigned char byte;

word combine(byte leftByte, byte rightByte);

class Chip8 {
public:
//private:
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
    // Private functions for executing instructions
    void execute(word);
    void opClear();
    void opJump(word NNN);
    void opSetRegister(byte X, word NN);
    void opAdd(byte X, word NN);
    void opSetIndex(word NNN);
    void opDraw(byte X, byte Y, byte N);
    void opCall(word NNN);
    void opReturn();
    void opSkipByteEqual(byte X, byte NN);
    void opSkipByteUnequal(byte X, byte NN);
    void opSkipRegEqual(byte X, byte Y);
    void opSkipRegUnequal(byte X, byte Y);
    void opCopyRegister(byte X, byte Y);
    void opOr(byte X, byte Y);
    void opAnd(byte X, byte Y);
    void opXor(byte X, byte Y);
    void opAddReg(byte X, byte Y);
    void opSubLR(byte X, byte Y);
    void opSubRL(byte X, byte Y);
    void opLeftShift(byte X, byte Y);
    void opRightShift(byte X, byte Y);
    void opRandom(byte X, byte NN);
    void opSkipKeyDown(byte X);
    void opSkipKeyNotDown(byte X);
    void opDelayToReg(byte X);
    void opSetDelayTimer(byte X);
    void opSetSoundTimer(byte X);
    void opAddRegToIndex(byte X);
    void opGetKey(byte X);
    void opFontChar(byte X);
    void opBinaryCodedDecimal(byte X);
    void opRegistersToRam(byte X);
    void opRamToRegisters(byte X);
//public:
    // Display buffer: displayBuffer[y][x] = T / F
    byte displayBuffer[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];
    bool draw;
    bool sound;
    bool copyBeforeShifting;
    byte keyState[16];
    Chip8();
    void cycle();
    void reset();
    void load(byte * rom);
    void dumpState();
    void dumpDisplay();
};

#endif // CHIP8_HPP