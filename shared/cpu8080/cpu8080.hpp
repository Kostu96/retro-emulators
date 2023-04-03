#pragma once
#include <ccl/non_copyable.h>
#include <ccl/types.hpp>

#include <functional>

class CPU8080 :
    public ccl::NonCopyable
{
public:
    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    CPU8080() = default;

    void reset();
    bool interrupt(u8 vector);
    void clock();
private:
    union Flags {
        struct {
            u8 Carry     : 1; // 0
            u8 unused1   : 1; // 1
            u8 Parity    : 1; // 2
            u8 unused3   : 1; // 3
            u8 HalfCarry : 1; // 4
            u8 unused5   : 1; // 5
            u8 Sign      : 1; // 6
            u8 Zero      : 1; // 7
        };

        u8 byte;
    };

    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(--SP, data); }
    void push16(u16 data) { store16(SP - 2, data); SP -= 2; }
    u8 pop8() { return load8(SP++); }
    u16 pop16() { SP += 2; return load16(SP - 2); }
    void add8(u8 value, u8 carry, u16& result, u8& result4bit);
    void add16(u16 value, u32& result, u16& result12bit);
    void subtract8(u8 value, u8 carry, u16& result, u8& result4bit);
    void compare8(u8 value, u16& result, u8& result4bit);
    void rotateLeft(u8& reg, u8& newCarry);
    void rotateLeftWithCarry(u8& reg, u8 carry, u8& newCarry);
    void rotateRight(u8& reg, u8& newCarry);
    void rotateRightWithCarry(u8& reg, u8 carry, u8& newCarry);

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

    u8 m_cyclesLeft;
    u8 m_interruptVector;
    bool m_interruptRequested;
    bool m_interruptEnabled;
    bool m_isHalted;
    bool m_conditionalTaken;
    bool m_EIRequested;
};
