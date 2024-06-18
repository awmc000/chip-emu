'''
void Chip8::opBinaryCodedDecimal(byte X) {
    // At I, put the hundreds
    ram[indexRegister] = X / 100;
    // At I + 1, put the tens
    ram[indexRegister + 1] = (X / 10) % 10;
    // At I + 2, put the ones
    ram[indexRegister + 2] = X % 10;
}
'''
def bcd(x):
    print(f'i: {x // 100}')
    print(f'i+1: {x // 10 % 10}')
    print(f'i+2: {x % 10}')