#include "chip8.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Overflow registers by adding", "[Class Members]") {
    Chip8 chip{};
    chip.variableRegisters[0] = 0xFF;
    REQUIRE(++chip.variableRegisters[0] == 0);
}

TEST_CASE("Overflow registers by subtracting", "[Class Members]") {
    Chip8 chip{};
    chip.variableRegisters[0] = 0x00;
    REQUIRE(--chip.variableRegisters[0] == 0xFF);
}

TEST_CASE("Jump to an address", "[Opcodes]") {
    Chip8 chip{};
    chip.opJump(0xB0B);
    REQUIRE(chip.programCounter == 0xB0B);
}

TEST_CASE("Set register", "[Opcodes]") {
    Chip8 chip{};
    chip.opSetRegister(0x0, 0xBA);
    REQUIRE(chip.variableRegisters[0x0] == 0xBA);
}

TEST_CASE("Add immediate value to register", "[Opcodes]") {
    Chip8 chip{};
    chip.opAdd(0x0, 0xBA);
    REQUIRE(chip.variableRegisters[0x0] == 0xBA);
}

TEST_CASE("Set index register", "[Opcodes]") {
    Chip8 chip{};
    chip.opSetIndex(0xFFF);
    REQUIRE(chip.indexRegister == 0xFFF);
}

TEST_CASE("Draw a row of a sprite", "[Opcodes]") {
    Chip8 chip{};
    chip.ram[0xFFF] = 0xFF;
    chip.indexRegister = 0xFFF;
    chip.opDraw(0x00, 0x00, 1);
    REQUIRE(chip.displayBuffer[0][0] == 1);
    REQUIRE(chip.displayBuffer[0][1] == 1);
    REQUIRE(chip.displayBuffer[0][2] == 1);
    REQUIRE(chip.displayBuffer[0][3] == 1);
    REQUIRE(chip.displayBuffer[0][4] == 1);
    REQUIRE(chip.displayBuffer[0][5] == 1);
    REQUIRE(chip.displayBuffer[0][6] == 1);
    REQUIRE(chip.displayBuffer[0][7] == 1);
}

TEST_CASE("Call a subroutine", "[Opcodes]") {
    Chip8 chip{};
    chip.opCall(0xFFF);
    REQUIRE(chip.programCounter == 0xFFF);
}

TEST_CASE("Return from a subroutine", "[Opcodes]") {
    Chip8 chip{};
    chip.stackPointer = 1;
    chip.stack[0] = 0xFFF;
    chip.opReturn();
    REQUIRE(chip.programCounter == 0xFFF);
}

TEST_CASE("Skip if vx == NN: true case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xC] = 0xD;
    chip.opSkipByteEqual(0xC, 0x0D);
    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if vx == NN: false case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xC] = 0xD;
    chip.opSkipByteEqual(0xC, 0x0E);
    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Skip if vx != NN: true case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xC] = 0xE;
    chip.opSkipByteUnequal(0xC, 0x0D);
    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if vx != NN: false case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xC] = 0xD;
    chip.opSkipByteUnequal(0xC, 0x0D);
    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Skip if vx == vy: true case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xA] = 0xB;
    chip.variableRegisters[0xC] = 0xB;
    chip.opSkipRegEqual(0xA, 0xC);
    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if vx == vy: False case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xA] = 0xB;
    chip.variableRegisters[0xC] = 0xC;
    chip.opSkipRegEqual(0xA, 0xC);
    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Skip if vx != vy: true case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xA] = 0xB;
    chip.variableRegisters[0xC] = 0xD;
    chip.opSkipRegUnequal(0xA, 0xC);
    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if vx != vy: False case", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xA] = 0xB;
    chip.variableRegisters[0xC] = 0xB;
    chip.opSkipRegUnequal(0xA, 0xC);
    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Copy register contents", "[Opcodes]") {
    Chip8 chip{};
    chip.variableRegisters[0xA] = 0xA;
    chip.variableRegisters[0xC] = 0xB;
    chip.opCopyRegister(0xA, 0xC);
    REQUIRE(chip.variableRegisters[0xA] == 0xB);
}

TEST_CASE("Bitwise logic operations", "[Opcodes]") {
    Chip8 chip{};
    
    chip.variableRegisters[0x0] = 0x01;
    chip.variableRegisters[0x1] = 0x01;
    chip.variableRegisters[0x2] = 0x00;

    chip.opAnd(0x0, 0x1);
    REQUIRE(chip.variableRegisters[0x0] == 0x01);

    chip.opOr(0x1, 0x2);
    REQUIRE(chip.variableRegisters[0x1] == 0x01);

    chip.opXor(0x2, 0x1);
    REQUIRE(chip.variableRegisters[0x2] == 0x01);

}

TEST_CASE("Add registers VX = VX + VY", "[Opcodes]") {
    Chip8 chip{};

    chip.variableRegisters[0x01] = 0x01;
    chip.variableRegisters[0x02] = 0x02;
    chip.opAddReg(0x01, 0x02);

    REQUIRE(chip.variableRegisters[0x01] == 3);
}

TEST_CASE("Subtract registers VX = VX - VY", "[Opcodes]") {
    Chip8 chip{};

    chip.variableRegisters[0x01] = 0x04;
    chip.variableRegisters[0x02] = 0x03;
    chip.opSubLR(0x01, 0x02);

    REQUIRE(chip.variableRegisters[0x01] == 1);

    // VF should be set to one as there was no overflow
    REQUIRE(chip.variableRegisters[0xF] == 1);
}

TEST_CASE("Subtract registers VX = VY - VX", "[Opcodes]") {
    Chip8 chip{};

    chip.variableRegisters[0x01] = 0x04;
    chip.variableRegisters[0x02] = 0x03;
    chip.opSubRL(0x01, 0x02);

    REQUIRE(chip.variableRegisters[0x01] == 0xFF);

    // VF should be set to zero as there was an overflow
    REQUIRE(chip.variableRegisters[0xF] == 0);
}


TEST_CASE("Left shift: VX = VY << 1, copy before shift", "[Opcodes]") {
    Chip8 chip{};

    chip.copyBeforeShifting = true;

    chip.variableRegisters[0x01] = 0x84;

    chip.opLeftShift(0x00, 0x01);

    REQUIRE(chip.variableRegisters[0x00] == 0x08);
    REQUIRE(chip.variableRegisters[0xF] == 1);
}

TEST_CASE("Left shift: VX = VX << 1", "[Opcodes]") {
    Chip8 chip{};

    chip.copyBeforeShifting = false;

    chip.variableRegisters[0x00] = 0x04;

    chip.opLeftShift(0x00, 0x01);

    REQUIRE(chip.variableRegisters[0x00] == 0x08);
    REQUIRE(chip.variableRegisters[0xF] == 0);
}


TEST_CASE("Right shift: Corax test", "[Opcodes]") {
    Chip8 chip{};

    chip.copyBeforeShifting = true;

    chip.variableRegisters[0x06] = 0xF;

    chip.opRightShift(0x06, 0x06);

    REQUIRE(chip.variableRegisters[0x06] == 0x07);
    REQUIRE(chip.variableRegisters[0xF] == 1);
}

TEST_CASE("Right shift: VX = VY >> 1", "[Opcodes]") {
    Chip8 chip{};

    chip.copyBeforeShifting = true;

    chip.variableRegisters[0x0] = 5;
    chip.variableRegisters[0x1] = 17;
    chip.opRightShift(0x0, 0x1);

    REQUIRE(chip.variableRegisters[0x0] == 8);
    REQUIRE(chip.variableRegisters[0xF] == 1);
}

TEST_CASE("Right shift: VX = VX >> 1", "[Opcodes]") {
    Chip8 chip{};

    chip.copyBeforeShifting = false;

    chip.variableRegisters[0x1] = 17;
    chip.opRightShift(0x1, 0x5);

    REQUIRE(chip.variableRegisters[0x1] == 8);
    REQUIRE(chip.variableRegisters[0xF] == 1);
}

TEST_CASE("Random number", "[Opcodes]") {
    Chip8 chip{};

    chip.variableRegisters[0x0] = 0;
    chip.opRandom(0x0, 0xFF);

    REQUIRE(chip.variableRegisters[0x0] != 0);
}

TEST_CASE("Skip if key down: true case", "[Opcodes]") {
    Chip8 chip{};

    chip.keyState[0x0] = 1;
    chip.opSkipKeyDown(0x0);

    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if key down: false case", "[Opcodes]") {
    Chip8 chip{};

    chip.keyState[0x0] = 0;
    chip.opSkipKeyDown(0x0);

    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Skip if key not down: true case", "[Opcodes]") {
    Chip8 chip{};

    chip.keyState[0x0] = 0;
    chip.opSkipKeyNotDown(0x0);

    REQUIRE(chip.programCounter == 0x202);
}

TEST_CASE("Skip if key not down: false case", "[Opcodes]") {
    Chip8 chip{};

    chip.keyState[0x0] = 1;
    chip.opSkipKeyNotDown(0x0);

    REQUIRE(chip.programCounter == 0x200);
}

TEST_CASE("Binary coded decimal", "[Opcodes]") {
    Chip8 chip{};

    chip.indexRegister = 0xB00;
    chip.variableRegisters[0xE] = 0xFF;
    chip.opBinaryCodedDecimal(0xE);

    REQUIRE(chip.ram[0xB00] == 0x02);
    REQUIRE(chip.ram[0xB00 + 1] == 0x05);
    REQUIRE(chip.ram[0xB00 + 2] == 0x05);
}