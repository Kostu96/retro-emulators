#pragma once
#include "../types.hpp"

#include <cassert>
#include <functional>
#include <vector>

class CPUx80Base
{
public:
    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    void reset();
    bool interrupt(u8 vector);

    u16 getAF() const { return m_state.AF; }
    void setAF(u16 value) { m_state.AF = value; }
    u16 getBC() const { return m_state.BC; }
    void setBC(u16 value) { m_state.BC = value; }
    u16 getDE() const { return m_state.DE; }
    void setDE(u16 value) { m_state.DE = value; }
    u16 getHL() const { return m_state.HL; }
    void setHL(u16 value) { m_state.HL = value; }
    u16 getSP() const { return m_state.SP; }
    void setSP(u16 value) { m_state.SP = value; }
    u16 getPC() const { return m_state.PC; }
    void setPC(u16 value) { m_state.PC = value; }
    u8 getCyclesLeft() const { return m_cyclesLeft; }
    bool isHalted() const { return m_state.IsHalted; }
    bool isHandlingInterrupt() const { return m_interruptRequested; }
protected:
    union Flags {
        struct {
            u8 alwaysZero : 4; // 0-3
            u8 Carry      : 1; // 4
            u8 HalfCarry  : 1; // 5
            u8 Subtract   : 1; // 6
            u8 Zero       : 1; // 7
        };

        u8 byte;
    };

    struct State
    {
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

        bool InterruptEnabled;
        bool IsHalted;
    };

    CPUx80Base() = default;
    virtual ~CPUx80Base() = default;

    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(--m_state.SP, data); }
    void push16(u16 data) { store16(m_state.SP - 2, data); m_state.SP -= 2; }
    u8 pop8() { return load8(m_state.SP++); }
    u16 pop16() { m_state.SP += 2; return load16(m_state.SP - 2); }

    State m_state;
    u8 m_interruptVector;
    bool m_interruptRequested;
    bool m_prefixMode;
    bool m_conditionalTaken;
    bool m_EIRequested;
    u8 m_cyclesLeft;
};

enum class CPUx80Mode : u8
{
    Intel8080,
    Z80,
    GameBoy
};

template <CPUx80Mode Mode>
class CPUx80 :
    public CPUx80Base
{
public:
    void clock();

    explicit CPUx80(CPUx80Mode mode) : m_mode{ mode } {}
    virtual ~CPUx80() = default;
private:
    u8 getCarryFlag();
    void setCarryFlag(u8 value);
    u8 getHalfCarryFlag();
    void setHalfCarryFlag(u8 value);
    u8 getSubtractFlag() { assert(false); return 0; }
    void setSubtractFlag(u8 /*value*/) { assert(false); }
    u8 getZeroFlag();
    void setZeroFlag(u8 value);
    u8 getSignFlag() { assert(false); return 0; }
    void setSignFlag(u8 /*value*/) { assert(false); }
    u8 getParityFlag() { assert(false); return 0; }
    void setParityFlag(u8 /*value*/) { assert(false); }

    void standardInstruction(u8 opcode);
    void prefixInstruction(u8 opcode);

    // CB prefix instructions:
    void RLC(u8& reg);
    void RLCM();
    void RRC(u8& reg);
    void RRCM();
    void RL(u8& reg);
    void RLM();
    void RR(u8& reg);
    void RRM();
    void SLA(u8& reg);
    void SLAM();
    void SRA(u8& reg);
    void SRAM();
    void SWAP(u8& reg);
    void SWAPM();
    void SRL(u8& reg);
    void SRLM();
    void BIT(u8 value, u8 bit);
    void RES(u8& reg, u8 bit);
    void RESM(u8 bit);
    void SET(u8& reg, u8 bit);
    void SETM(u8 bit);

    void XCHG();
    void XTHL();
    void ADD(u8 value);
    void ADC(u8 value);
    void ADDHL(u16 value);
    void SUB(u8 value);
    void SBB(u8 value);
    void CMP(u8 value);
    void DECR(u8& reg);
    void DECM();
    void INCR(u8& reg);
    void INCM();
    void RLCA();
    void RRCA();
    void RAR();
    void RAL();
    void DAA();
    void AND(u8 value);
    void OR(u8 value);
    void XOR(u8 value);
    void JMP(bool flag);
    void JR(bool flag);
    void CALL(bool flag);
    void RET(bool flag);
    void RST(u8 vector);

    const CPUx80Mode m_mode;
};
