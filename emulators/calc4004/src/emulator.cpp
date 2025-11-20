#include "emulator.hpp"

#include <cpu40xx/assembler40xx.hpp>
#include <utils/file_io.hpp>

#include <cassert>
#include <charconv>
#include <iostream>

namespace calc4004 {

Emulator::Emulator() :
    cpu_(CPU40xx::Mode::Intel4004) {
    cpu_.mapReadROMCallback([this](u16 address) { return read_ROM(address); });
    cpu_.mapReadRAMDataCallback([this](u8 cm_ram) { return read_RAM_data(cm_ram); });
    cpu_.mapWriteRAMDataCallback([this](u8 cm_ram, u8 value) { write_RAM_data(cm_ram, value); });
    cpu_.mapReadRAMStatusCallback([this](u8 cm_ram, u8 char_idx) { return read_RAM_status(cm_ram, char_idx); });
    cpu_.mapWriteRAMStatusCallback([this](u8 cm_ram, u8 char_idx, u8 value) { write_RAM_status(cm_ram, char_idx, value); });
    cpu_.mapWriteRAMIOCallback([this](u8 cm_ram, u8 value) { write_RAM_output(cm_ram, value); });
    cpu_.mapWriteSRCRegisterCallback([this](u8 cm_ram, u8 value) {
        assert(cm_ram == 1);
        ROM_SRC_reg_ = value; // TODO(Kostu): save only what ROM chips really have in hardware
        RAM_SRC_reg_ = value; // TODO(Kostu): same here
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

    // handle shift register
    assert(false);
}

} // namespace calc4004
