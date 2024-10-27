#pragma once
#include "shared/source/types.hpp"

#include <functional>

class CPU8080
{
public:
    union Flags {
        struct {
            u8 Carry          : 1; // 0
            u8 unused1        : 1; // 1
            u8 Parity         : 1; // 2
            u8 unused3        : 1; // 3
            u8 AuxiliaryCarry : 1; // 4
            u8 unused5        : 1; // 5
            u8 Sign           : 1; // 6
            u8 Zero           : 1; // 7
        };

        u8 byte;
    };

    struct State {
        u16 PC;
        u16 SP;
        union {
            struct {
                Flags F;
                u8 A;
            };
            u16 AF;
        };
        union {
            struct {
                u8 C, B;
            };
            u16 BC;
        };
        union {
            struct {
                u8 E, D;
            };
            u16 DE;
        };
        union {
            struct {
                u8 L, H;
            };
            u16 HL;
        };
    };

    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    void reset();
    bool interrupt(u8 vector);
    void clock();

    const State& getState() const { return m_state; }
    State& getState() { return m_state; }
    u8 getCyclesLeft() const { return m_cyclesLeft; }

    CPU8080() = default;
    CPU8080(CPU8080&) = delete;
    CPU8080& operator=(CPU8080&) = delete;
private:
    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(--m_state.SP, data); }
    void push16(u16 data) { store16(m_state.SP - 2, data); m_state.SP -= 2; }
    u8 pop8() { return load8(m_state.SP++); }
    u16 pop16() { m_state.SP += 2; return load16(m_state.SP - 2); }

    void executeInstruction(u8 opcode);

    void ADD(u8 value);
    void ADC(u8 value);
    void ADDHL(u16 value);
    void AND(u8 value);
    void CALL(bool flag);
    void CMP(u8 value);
    void DAA();
    void DECM();
    void DECR(u8& reg);
    void INCM();
    void INCR(u8& reg);
    void JMP(bool flag);
    void OR(u8 value);
    void RAL();
    void RAR();
    void RET(bool flag);
    void RLC();
    void RRC();
    void RST(u8 vector);
    void SUB(u8 value);
    void SBB(u8 value);
    void XCHG();
    void XOR(u8 value);
    void XTHL();

    State m_state;

    u8 m_cyclesLeft;
    u8 m_interruptVector;
    bool m_interruptRequested;
    bool m_interruptEnabled;
    bool m_isHalted;
    bool m_conditionalTaken;
    bool m_EIRequested;
};
