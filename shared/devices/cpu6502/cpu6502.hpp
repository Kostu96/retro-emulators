#pragma once
#include "../../types.hpp"

#include <functional>

class CPU6502
{
public:
    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    union Flags
    {
        struct {
            u8 C : 1; // 0
            u8 Z : 1; // 1
            u8 I : 1; // 2
            u8 D : 1; // 3
            u8 B : 1; // 4
            u8 U : 1; // 5
            u8 V : 1; // 6
            u8 N : 1; // 7
        } bits;
        u8 byte;
    };

    void reset();
    void clock();
    void setIRQ(bool state) { m_irq = state; }
    void setNMI(bool state) { m_nmi = state; }

    void setPC(u16 value) { PC = value; }
    u16 getPC() const { return PC; }
    Flags getFlags() const { return F; }

    // test only:
    void runUntilEndlessLoop();

    CPU6502() = default;
    CPU6502(CPU6502&) = delete;
    CPU6502& operator=(CPU6502&) = delete;
private:
    void IRQ();
    void NMI();

    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(0x100 + SP--, data); }
    void push16(u16 data) { store16(0x100 + SP - 1, data); SP -= 2; }
    u8 pop8() { return load8(0x100 + ++SP); }
    u16 pop16() { SP += 2; return load16(0x100 + SP - 1); }

    // Addressing Modes:
    void am_ACC(); void am_IMM();
    void am_ZPG(); void am_ZPX(); void am_ZPY();
    void am_ABS(); void am_ABX(); void am_ABY();
    void am_IND(); void am_INX(); void am_INY();

    void op_NOP();

    // Branch Instructions:
    void op_JMP();
    void op_BPL(); void op_BMI();
    void op_BEQ(); void op_BNE();
    void op_BCS(); void op_BCC();
    void op_BVS(); void op_BVC();
    void op_JSR(); void op_RTS();
    void op_BRK(); void op_RTI();
    
    // Load/Store Instructions:
    void op_LDA(); void op_LDX(); void op_LDY();
    void op_STA(); void op_STX(); void op_STY();

    // Transfer Instructions:
    void op_TXA(); void op_TAX();
    void op_TYA(); void op_TAY();
    void op_TSX(); void op_TXS();

    // Stack Instructions:
    void op_PHA(); void op_PLA();
    void op_PHP(); void op_PLP();

    // ALU Instructions:
    void op_ADC(); void op_SBC();
    void op_ORA(); void op_AND(); void op_EOR();
    void op_LSR(); void op_ASL();
    void op_ROL(); void op_ROR();
    void op_BIT();
    void op_INC(); void op_INX(); void op_INY();
    void op_DEC(); void op_DEX(); void op_DEY();
    void op_CMP(); void op_CPX(); void op_CPY();

    // Flags Instructions:
    void op_SEC(); void op_CLC();
    void op_SEI(); void op_CLI();
    void op_SED(); void op_CLD();
    void op_CLV();

    // CPU State:
    u16 PC;
    u8 ACC;
    Flags F;
    u8 X;
    u8 Y;
    u8 SP;

    // Helpers:
    mutable u16 m_cyclesLeft;
    u16 m_absoluteAddress;
    bool m_isACCAddressing = false;
    bool m_irq = false;
    bool m_nmi = false;
    bool m_isDuringNMI = false;
};
