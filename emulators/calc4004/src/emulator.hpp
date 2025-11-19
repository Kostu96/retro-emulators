#pragma once
#include <cpu40xx/cpu40xx.hpp>

namespace calc4004 {

constexpr s64 INSTRUCTION_TIME = 10800; // nanoseconds

class Emulator {
public:
    Emulator();

    void clock() { cpu_.clock(); }
private:
    static constexpr size_t NUM_ROM_CHIPS = 2;
    static constexpr size_t ROM_SIZE = NUM_ROM_CHIPS * 0x100;
    static constexpr size_t NUM_RAM_CHIPS = 4;
    static constexpr size_t NUM_RAM_REGISTERS = NUM_RAM_CHIPS * 4;
    static constexpr size_t RAM_DATA_SIZE = NUM_RAM_REGISTERS * 16;
    static constexpr size_t RAM_STATUS_SIZE = NUM_RAM_REGISTERS * 4;

    u8 read_ROM(u16 address) const;
    u8 read_RAM_data(u8 cm_ram) const;
    void write_RAM_data(u8 cm_ram, u8 value);
    u8 read_RAM_status(u8 cm_ram, u8 char_idx);
    void write_RAM_status(u8 cm_ram, u8 char_idx, u8 value);

    CPU40xx cpu_;
    u8 ROM_[ROM_SIZE]{};
    u8 ROM_SRC_reg_{};
    u8 RAM_data_[RAM_DATA_SIZE]{};
    u8 RAM_status_[RAM_STATUS_SIZE]{};
    u8 RAM_SRC_reg_{};
};

} // namespace calc4004
