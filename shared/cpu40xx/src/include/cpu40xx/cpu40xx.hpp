#pragma once
#include "utils/types.hpp"
#include "utils/warnings.hpp"

#include <functional>
#include <vector>

class CPU40xx
{
public:

BEGIN_ALLOW_ANON_STRUCTS
    struct State {
        std::vector<u8> regs;
        std::vector<u16> stack;
        u8 ACC : 4;
        u8 CMRAM : 4;
        u8 SP : 3;
        u8 CY : 1;
        u8 test : 1;
        union {
            struct {
                u8 RAMCharIdx : 4;
                u8 RAMRegIdx  : 2;
                u8 RAMChip    : 2;
            };
            struct {
                u8 unused  : 4;
                u8 ROMChip : 4;
            };
            u8 SRCReg;
        };
    };
END_ALLOW_ANON_STRUCTS

    using ReadROMCallback = std::function<u8(u16)>;
    using ReadRAMCallback = std::function<u8(u8)>;
    using WriteRAMCallback = std::function<void(u8, u8)>;
    using ReadIOCallback = std::function<u8(u8)>;
    using WriteIOCallback = std::function<void(u8, u8)>;
    using ReadStatusCallback = std::function<u8(u8)>;
    using WriteStatusCallback = std::function<void(u8, u8)>;
    void mapReadROMCallback(ReadROMCallback callback) { loadROM8 = callback; }
    void mapReadRAMCallback(ReadRAMCallback callback) { loadRAM8 = callback; }
    void mapWriteRAMCallback(WriteRAMCallback callback) { storeRAM8 = callback; }
    void mapReadIOCallback(ReadIOCallback callback) { loadIO8 = callback; }
    void mapWriteIOCallback(WriteIOCallback callback) { storeIO8 = callback; }
    void mapReadRAMStatus(ReadStatusCallback func) { loadStatus8 = func; }
    void mapWriteRAMStatus(WriteStatusCallback func) { storeStatus8 = func; }

    enum class Mode : u8 {
        Intel4004,
        Intel4040
    };

    void reset();
    void clock();

    const State& getState() const { return m_state; }
    State& getState() { return m_state; }
    u16 getPC() const { return m_state.stack[m_state.SP]; }
    u16& getPC() { return m_state.stack[m_state.SP]; }

    explicit CPU40xx(Mode mode);
    CPU40xx(const CPU40xx&) = delete;
    CPU40xx& operator=(const CPU40xx&) = delete;
private:
    ReadROMCallback loadROM8 = nullptr;
    ReadRAMCallback loadRAM8 = nullptr;
    WriteRAMCallback storeRAM8 = nullptr;
    ReadIOCallback loadIO8 = nullptr;
    WriteIOCallback storeIO8 = nullptr;
    ReadStatusCallback loadStatus8 = nullptr;
    WriteStatusCallback storeStatus8 = nullptr;

    u8 getRAMAddress() const { return (m_state.CMRAM >> 1) | m_state.SRCReg; }
    void incPC() { m_state.stack[m_state.SP]++; m_state.stack[m_state.SP] &= 0xFFF; }
    void incSP() { m_state.SP++; m_state.SP &= (m_mode == Mode::Intel4004 ? 0b11 : 0b111); }
    void decSP() { m_state.SP--; m_state.SP &= (m_mode == Mode::Intel4004 ? 0b11 : 0b111); }

    void ADD(u8 idx);
    void ADM();
    void BBL(u8 data);
    void CLB();
    void CLC();
    void CMA();
    void CMC();
    void DAA();
    void DAC();
    void DCL();
    void FIM(u8* reg);
    void FIN(u8 idx);
    void IAC();
    void INC(u8 idx);
    void ISZ(u8 idx);
    void JCN(u8 condition);
    void JIN(u8 idx);
    void JMS(u16 highNibble);
    void JUN(u16 highNibble);
    void KBP();
    void LD(u8 idx);
    void LDM(u8 data);
    void RAL();
    void RAR();
    void RDM();
    void RDR();
    void RDX(u8 charIdx);
    void SBM();
    void SRC(u8 idx);
    void STC();
    void SUB(u8 idx);
    void TCC();
    void TCS();
    void WMP();
    void WRM();
    void WRR();
    void WRX(u8 charIdx);
    void XCH(u8 idx);

    const Mode m_mode;
    State m_state;
};
