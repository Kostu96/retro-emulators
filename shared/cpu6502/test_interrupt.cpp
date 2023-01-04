#include "cpu6502.hpp"

#include <fstream>

static struct {
    u8 memory[0xFFF7];

    u8 read(u16 address) const { return memory[address]; }
    void write(u16 address, u8 data) { memory[address] = data; }
} MEMORY;

class FeedbackRegister
{
public:
    explicit FeedbackRegister(CPU6502& cpu) : m_cpu{ cpu } {}

    u8 read(u16 address) const
    {
        return reg;
    }

    void write(u16 address, u8 data)
    {
        reg = data;
        m_cpu.setIRQ(reg & 1);
        m_cpu.setNMI(reg & 2);
    }
private:
    u8 reg = 0;
    CPU6502& m_cpu;
};

static struct {
    u8 memory[6];

    u8 read(u16 address) const { return memory[address]; }
} VECTORS;

int main()
{
    std::ifstream fin{ "6502_interrupt_test.bin", std::ios::binary };
    if (fin.is_open())
    {
        fin.read((char*)(MEMORY.memory + 0xA), 0xFFF7 - 0xA);
        fin.ignore(3);
        fin.read((char*)VECTORS.memory, 6);
        fin.close();
    }
    else return -1;

    CPU6502 cpu{};
    FeedbackRegister reg{ cpu };

    cpu.map(MEMORY,  { 0x0000, 0xFFF6 });
    cpu.map(reg,     { 0xFFF7, 0xFFF8 });
    cpu.map(VECTORS, { 0xFFFA, 0xFFFF });

    u8 RSTVectorL = VECTORS.memory[2];
    u8 RSTVectorH = VECTORS.memory[3];
    VECTORS.memory[2] = 0x00;
    VECTORS.memory[3] = 0x04;
    cpu.reset();
    VECTORS.memory[2] = RSTVectorL;
    VECTORS.memory[3] = RSTVectorH;

    while (cpu.getPC() != 0x06F5)
        cpu.clock();

    return 0;
}
