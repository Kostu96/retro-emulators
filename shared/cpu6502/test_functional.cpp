#include "cpu6502.hpp"

#include <fstream>

static struct {
    u8 memory[0x10000];

    u8 read(u16 address) const { return memory[address]; }
    void write(u16 address, u8 data) { memory[address] = data; }
} MEMORY;

int main()
{
    std::ifstream fin{ "6502_functional_test.bin", std::ios::binary };
    if (fin.is_open())
    {
        fin.read((char*)MEMORY.memory, 0x10000);
        fin.close();
    }
    else return -1;

    CPU6502 cpu{};
    cpu.map(MEMORY, { 0x0000, 0xFFFF });

    u8 RSTVectorL = MEMORY.memory[0xFFFC];
    u8 RSTVectorH = MEMORY.memory[0xFFFD];
    MEMORY.memory[0xFFFC] = 0x00;
    MEMORY.memory[0xFFFD] = 0x04;
    cpu.reset();
    MEMORY.memory[0xFFFC] = RSTVectorL;
    MEMORY.memory[0xFFFD] = RSTVectorH;

    while (cpu.getPC() != 0x3469)
        cpu.clock();

    return 0;
}
