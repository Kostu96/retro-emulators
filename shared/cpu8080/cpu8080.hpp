#pragma once
#include "../type_aliases.hpp"

#include <functional>
#include <vector>

#if defined(CPU8080_TESTS)
#define PRIVATE public
#else
#define PRIVATE private
#endif

class CPU8080
{
public:
    enum class Mode : u8
    {
        Intel8080,
        Intel8085,
        Z80,
        GameBoy
    };

    using ReadMemoryCallback = std::function<u8(u16)>;
    using WriteMemoryCallback = std::function<void(u16, u8)>;
    void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
    void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

    void reset();
    void clock();
    void interrupt(u8 vector);

    u16 getAF() const { return m_state.AF; }
    u16 getBC() const { return m_state.BC; }
    u16 getDE() const { return m_state.DE; }
    u16 getHL() const { return m_state.HL; }
    u16 getSP() const { return m_state.SP; }
    u16 getPC() const { return m_state.PC; }

    explicit CPU8080(Mode mode) : m_mode{ mode }, m_state{ mode } {}
    CPU8080(const CPU8080&) = delete;
    CPU8080& operator=(const CPU8080&) = delete;
PRIVATE:
    class Flags
    {
    public:
        explicit Flags(Mode mode) : m_mode{ mode } {}

        u8 getCarry();
        void setCarry(u8 value);
        u8 getHalfCarry();
        void setHalfCarry(u8 value);
        u8 getSubtract();
        void setSubtract(u8 value);
        u8 getZero();
        void setZero(u8 value);
        u8 getSign();
        void setSign(u8 value);
        u8 getParity();
        void setParity(u8 value);
    private:
        union {
            struct {
                u8 alwaysZero : 4; // 0-3
                u8 Carry      : 1; // 4
                u8 HalfCarry  : 1; // 5
                u8 Subtract   : 1; // 6
                u8 Zero       : 1; // 7
            } m_gb;

            struct {
                u8 Carry     : 1; // 0
                u8 unused1   : 1; // 1
                u8 Parity    : 1; // 2
                u8 unused3   : 1; // 3
                u8 HalfCarry : 1; // 4
                u8 unused5   : 1; // 5
                u8 Sign      : 1; // 6
                u8 Zero      : 1; // 7
            } m_i8080;

            u8 m_byte;
        };

        Mode m_mode;
    };

    struct State
    {
        explicit State(Mode mode) : F{ mode } {}

        u16 PC;
        u16 SP;
        union {
            struct {
                u8 A;
                Flags F;
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
    };

    void standardInstruction(u8 opcode);
    void prefixInstruction(u8 opcode);

    ReadMemoryCallback load8 = nullptr;
    WriteMemoryCallback store8 = nullptr;
    u16 load16(u16 address) const { return load8(address) | (load8(address + 1) << 8); }
    void store16(u16 address, u16 data) const { store8(address, data & 0xFF); store8(address + 1, data >> 8); }
    void push8(u8 data) { store8(--m_state.SP, data); }
    void push16(u16 data) { store16(m_state.SP - 2, data); m_state.SP -= 2; }
    u8 pop8() { return load8(m_state.SP++); }
    u16 pop16() { m_state.SP += 2; return load16(m_state.SP - 2); }

    void BIT(u8 value, u8 bit);
    void LDR(u8& dst, u8 value);
    void LDRP(u16& dst, u16 value);
    void LDM(u16 address, u8 value);
    void XCHG();
    void XTHL();
    void ADD(u8 value);
    void ADC(u8 value);
    void SUB(u8 value);
    void SBB(u8 value);
    void ADDHL(u16 value);
    void CMP(u8 value);
    void DECR(u8& reg);
    void DECRP(u16& reg);
    void DECM();
    void INCR(u8& reg);
    void INCRP(u16& reg);
    void INCM();
    void RRC();
    void RLC();
    void RAR();
    void RAL();
    void DAA();
    void AND(u8 value);
    void OR(u8 value);
    void XOR(u8 value);
    void STC();
    void CMC();
    void CMA();
    void JMP(bool flag);
    void JR(bool flag);
    void CALL(bool flag);
    void RET(bool flag);
    void RST(u8 vector);

    State m_state;

    const Mode m_mode;
    bool m_prefixMode;
};
