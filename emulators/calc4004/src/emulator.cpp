#include "emulator.hpp"

#include <cassert>

namespace calc4004 {

Emulator::Emulator() :
    cpu_(CPU40xx::Mode::Intel4004) {
    cpu_.mapReadROMCallback([this](u16 address) { return read_ROM(address); });
    cpu_.mapReadRAMDataCallback([this](u8 cm_ram) { return read_RAM_data(cm_ram); });
    cpu_.mapWriteRAMDataCallback([this](u8 cm_ram, u8 value) { write_RAM_data(cm_ram, value); });
    cpu_.mapWriteSRCRegisterCallback([this](u8 cm_ram, u8 value) {
        assert(cm_ram == 1);
        ROM_SRC_reg_ = value; // TODO(Kostu): save only what ROM chips really have in hardware
        RAM_SRC_reg_ = value; // TODO(Kostu): same here
        });

    cpu_.reset();
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

} // namespace calc4004
