# chip-emu
Chip-8 Emulator implemented in C++ with SDL2
# Design
- Based on two classes, Chip8 and Frontend
- Chip8 acts strictly as backend: Frontend tells Chip8 when to run a CPU cycle, sends it key presses,
and draws the contents of its screen buffer.

# Instructions Supported
| Works?   | Instruction|
|----------|------------|
YES | 00E0: Clear screen
YES | 1NNN: Jump
NO  | 00EE and 2NNN: Subroutines
NO  | 3XNN, 4XNN, 5XY0 and 9XY0: Skip conditionally
YES | 6XNN: Set
YES | 7XNN: Add
NO  | 8XY0: Set
NO  | 8XY1: Binary OR
NO  | 8XY2: Binary AND
NO  | 8XY3: Logical XOR
NO  | 8XY4: Add
NO  |8XY5 and 8XY7: Subtract
NO  |8XY6 and 8XYE: Shift
YES|ANNN: Set index
NO  |BNNN: Jump with offset
NO  |CXNN: Random
YES| DXYN: Display
NO  |EX9E and EXA1: Skip if key
NO  |FX07, FX15 and FX18: Timers
NO  |FX1E: Add to index
NO  |FX0A: Get key
NO  |FX29: Font character
NO  |FX33: Binary-coded decimal conversion
NO  |FX55 and FX65: Store and load 