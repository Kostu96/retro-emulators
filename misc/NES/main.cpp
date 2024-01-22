#include "cpu6502.hpp"
#include "ppu2C02.hpp"
#include "cartridge.hpp"

static struct {
    u8 memory[0x800];

    u8 read(u16 address) const { return memory[address & 0x07FF]; }
    void write(u16 address, u8 data) { memory[address & 0x07FF] = data; }
} RAM;

int main()
{
    CPU6502 cpu{};
    PPU2C02 ppu{};
    Cartridge cart{};

    cpu.map(RAM,  { 0x0000, 0x1FFF });
    cpu.map(ppu,  { 0x2000, 0x3FFF });
    cpu.map(cart, { 0x4020, 0xFFFF });
    cart.load("nestest.nes");
    cpu.reset();
    ppu.reset();

    while (true)
    {
        cpu.clock();
        ppu.clock();
    }

    return 0;
}
