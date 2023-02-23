#pragma once
#include "../address_range.hpp"

#include <ccl/types.hpp>

#include <concepts>
#include <functional>
#include <vector>

class CPU6502
{
public:
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

    template <Mapable Device>
    void map(Device& device, AddressRange range);
    template <ConstMapable ConstDevice>
    void map(const ConstDevice& device, AddressRange range);

    void reset();
    void clock();
    void setIRQ(bool state) { m_irq = state; }
    void setNMI(bool state) { m_nmi = state; }

    u8 load8(u16 address) const;
    u16 getPC() const { return PC; }
    Flags getFlags() const { return F; }

    CPU6502() = default;
    CPU6502(const CPU6502&) = delete;
    CPU6502& operator=(const CPU6502&) = delete;
private:
    void IRQ();
    void NMI();

    // Memory Access:
    u16 load16(u16 address) const;
    void store8(u16 address, u8 data);
    void store16(u16 address, u16 data);
    void push8(u8 data);
    void push16(u16 data);
    u8 pop8();
    u16 pop16();

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

    std::vector<ReadMapEntry> m_readMap;
    std::vector<WriteMapEntry> m_writeMap;
};

template<Mapable Device>
inline void CPU6502::map(Device& device, AddressRange range)
{
    m_readMap.emplace_back(
        ReadMapEntry{
            range,
            [&device](u16 address) { return device.read(address); }
        }
    );
    m_writeMap.emplace_back(
        WriteMapEntry{
            range,
            [&device](u16 address, u8 data) { return device.write(address, data); }
        }
    );
}

template<ConstMapable ConstDevice>
inline void CPU6502::map(const ConstDevice& device, AddressRange range)
{
    m_readMap.emplace_back(
        ReadMapEntry{
            range,
            [&device](u16 address) { return device.read(address); }
        }
    );
}
