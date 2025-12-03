#include "emulator.hpp"

#include <cpu40xx/assembler40xx.hpp>
#include <utils/file_io.hpp>

#include <cassert>
#include <charconv>
#include <iostream>

namespace calc4004 {

void SR4003::clock(u8 data_in) {
    value_ <<= 1;
    value_ |= (data_in & 1);
}

Emulator::Emulator() :
    cpu_(CPU40xx::Mode::Intel4004) {
    cpu_.mapReadROMCallback([this](u16 address) { return read_ROM(address); });
    cpu_.mapWriteROMIOCallback([this](u8 value) { write_ROM_IO(value); });
    cpu_.mapReadRAMDataCallback([this](u8 cm_ram) { return read_RAM_data(cm_ram); });
    cpu_.mapWriteRAMDataCallback([this](u8 cm_ram, u8 value) { write_RAM_data(cm_ram, value); });
    cpu_.mapReadRAMStatusCallback([this](u8 cm_ram, u8 char_idx) { return read_RAM_status(cm_ram, char_idx); });
    cpu_.mapWriteRAMStatusCallback([this](u8 cm_ram, u8 char_idx, u8 value) { write_RAM_status(cm_ram, char_idx, value); });
    cpu_.mapWriteRAMIOCallback([this](u8 cm_ram, u8 value) { write_RAM_output(cm_ram, value); });
    cpu_.mapWriteSRCRegisterCallback([this](u8 cm_ram, u8 value) {
        assert(cm_ram == 1);
        ROM_SRC_reg_ = (value >> 4) & 0xF;
        RAM_SRC_reg_ = (value >> 6) & 0b11;
    });

    cpu_.reset();

    std::string rom_source = readFile("data/rom.asm");
    Assembler40xx assembler{ rom_source };
    [[maybe_unused]] auto status = assembler.assemble();
    assert(status == Assembler40xx::Status::Success);
    std::cout << assembler.getLog();
    std::string_view hex = assembler.getHex();
    while (true) {
        size_t colon_pos = hex.find_first_of(':');
        if (colon_pos == std::string_view::npos) {
            break;
        }
        hex = hex.substr(colon_pos + 1);
        u8 checksum_accumulator = 0;

        const char* start = hex.data();
        u8 data_length;
        std::from_chars(start, start + 2, data_length, 16);
        start += 2;
        checksum_accumulator += data_length;
        
        u16 address;
        std::from_chars(start, start + 4, address, 16);
        start += 4;
        checksum_accumulator += address & 0xFF;
        checksum_accumulator += (address >> 8) & 0xFF;
        assert(address + data_length <= ROM_SIZE);

        u8 type;
        std::from_chars(start, start + 2, type, 16);
        start += 2;
        checksum_accumulator += type;
        
        while (data_length--) {
            u8 byte;
            std::from_chars(start, start + 2, byte, 16);
            start += 2;
            checksum_accumulator += byte;

            ROM_[address++] = byte;
        }
        
        u8 checksum;
        std::from_chars(start, start + 2, checksum, 16);
        start += 2;
        checksum_accumulator += checksum;
        assert(checksum_accumulator == 0);
    }
}

u8 Emulator::read_ROM(u16 address) const {
    assert(address < ROM_SIZE);
    return ROM_[address];
}

void Emulator::write_ROM_IO(u8 value) {
    assert((value & 0xF0) == 0);
    assert(ROM_SRC_reg_ < 2);

    switch (ROM_SRC_reg_) {
    case 0: break;
    case 1: {// shift register
        static bool last_clock_state = false;
        bool clock_state = value & 0b10;
        if (!last_clock_state && clock_state) {
            shift_register.clock((value >> 2) & 1);
        }
        last_clock_state = clock_state;
        shift_register.enable(value & 1);
    } break;
    }
}

u8 Emulator::read_RAM_data(u8 cm_ram) const {
    assert(cm_ram == 1);
    u8 address = RAM_SRC_reg_; // TODO(Kostu): fix this
    assert(address < RAM_DATA_SIZE);
    return RAM_data_[address];
}

void Emulator::write_RAM_data(u8 cm_ram, u8 value) {
    assert(cm_ram == 1);
    assert((value & 0xF0) == 0);
    u8 address = RAM_SRC_reg_; // TODO(Kostu): fix this
    assert(address < RAM_DATA_SIZE);
    RAM_data_[address] = value;
}

u8 Emulator::read_RAM_status(u8 cm_ram, u8 char_idx) {
    assert(cm_ram == 1);
    u8 address = RAM_SRC_reg_ + char_idx; // // TODO(Kostu): fix this
    assert(address < RAM_STATUS_SIZE);
    return RAM_status_[address];
}

void Emulator::write_RAM_status(u8 cm_ram, u8 char_idx, u8 value) {
    assert(cm_ram == 1);
    assert((value & 0xF0) == 0);
    u8 address = RAM_SRC_reg_ + char_idx; // // TODO(Kostu): fix this
    assert(address < RAM_STATUS_SIZE);
    RAM_status_[address] = value;
}

void Emulator::write_RAM_output(u8 cm_ram, u8 value) {
    assert(cm_ram == 1);
    assert((value & 0xF0) == 0);
    assert((RAM_SRC_reg_ & 0xFC) == 0);

    switch (RAM_SRC_reg_) {
    case 0: assert(false); break;
    case 1: assert(false); break;
    case 2:
        assert(false);
        break;
    case 3: assert(false); break;
    }
}

} // namespace calc4004
