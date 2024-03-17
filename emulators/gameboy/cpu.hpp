#pragma once
#include "../../shared/types.hpp"

#include <functional>

class CPU
{
public:
    union Flags {
        struct {
            u8 alwaysZero : 4; // 0-3
            u8 Carry : 1;      // 4
            u8 HalfCarry : 1;  // 5
            u8 Subtract : 1;   // 6
            u8 Zero : 1;       // 7
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

    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    void reset();
    bool interrupt(u8 vector);
    void clock();

    const State& getState() const { return m_state; }
    void setCarry(u8 value) { m_state.F.Carry = value; }
    void setHalfCarry(u8 value) { m_state.F.HalfCarry = value; }
    void setSubtract(u8 value) { m_state.F.Subtract = value; }
    void setZero(u8 value) { m_state.F.Zero = value; }
    void setA(u8 value) { m_state.A = value; }
    void setB(u8 value) { m_state.B = value; }
    void setC(u8 value) { m_state.C = value; }
    void setD(u8 value) { m_state.D = value; }
    void setE(u8 value) { m_state.E = value; }
    void setH(u8 value) { m_state.H = value; }
    void setL(u8 value) { m_state.L = value; }
    void setAF(u16 value) { m_state.AF = value; }
    void setBC(u16 value) { m_state.BC = value; }
    void setDE(u16 value) { m_state.DE = value; }
    void setHL(u16 value) { m_state.HL = value; }
    void setSP(u16 value) { m_state.SP = value; }
    void setPC(u16 value) { m_state.PC = value; }
    u8 getCyclesLeft() const { return m_cyclesLeft; }
    bool isHandlingInterrupt() const { return m_interruptRequested; }
    
    CPU() = default;
    CPU(const CPU&) = delete;
    CPU& operator=(const CPU&) = delete;
private:
    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(--m_state.SP, data); }
    void push16(u16 data) { store16(m_state.SP - 2, data); m_state.SP -= 2; }
    u8 pop8() { return load8(m_state.SP++); }
    u16 pop16() { m_state.SP += 2; return load16(m_state.SP - 2); }

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

    // standard instructions
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

    State m_state;
    u8 m_interruptVector;
    bool m_interruptRequested;
    bool m_prefixMode;
    bool m_conditionalTaken;
    bool m_EIRequested;
    u8 m_cyclesLeft;
};
