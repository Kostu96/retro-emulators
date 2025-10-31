#pragma once
#include "utils/types.hpp"
#include "utils/warnings.hpp"

#include <functional>
#include <vector>

class CPU40xx
{
public:
    struct State {
        std::vector<u8> regs;
        std::vector<u16> stack;
        u8 ACC : 4;
        u8 CMRAM : 4;
        u8 SP : 3;
        u8 CY : 1;
        u8 test : 1;
    };

    using ReadROMCallback = std::function<u8(u16)>;
    using ReadU8Callback = std::function<u8(u8)>;
    using ReadU8U8Callback = std::function<u8(u8, u8)>;
    using WriteU8Callback = std::function<void(u8, u8)>;
    using WriteU8U8Callback = std::function<void(u8, u8, u8)>;
    void mapReadROMCallback(ReadROMCallback callback) { loadROM8 = callback; }
    void mapWriteSRCRegisterCallback(WriteU8Callback func) { storeSRCReg = func; }
    void mapReadRAMDataCallback(ReadU8Callback callback) { loadRAMData8 = callback; }
    void mapWriteRAMDataCallback(WriteU8Callback callback) { storeRAMData8 = callback; }
    void mapReadRAMStatusCallback(ReadU8U8Callback func) { loadRAMStatus8 = func; }
    void mapWriteRAMStatusCallback(WriteU8U8Callback func) { storeRAMStatus8 = func; }
    void mapReadIOCallback(ReadU8Callback callback) { loadIO8 = callback; }
    void mapWriteIOCallback(WriteU8Callback callback) { storeIO8 = callback; }

    enum class Mode : u8 {
        Intel4004,
        Intel4040
    };

    void reset();
    void clock();

    const State& getState() const { return m_state; }
    State& getState() { return m_state; }
    u16 getPC() const { return m_state.stack[m_state.SP]; }

    explicit CPU40xx(Mode mode);
    CPU40xx(const CPU40xx&) = delete;
    CPU40xx& operator=(const CPU40xx&) = delete;
private:
    ReadROMCallback loadROM8 = nullptr;
    WriteU8Callback storeSRCReg = nullptr;
    ReadU8Callback loadRAMData8 = nullptr;
    WriteU8Callback storeRAMData8 = nullptr;
    ReadU8U8Callback loadRAMStatus8 = nullptr;
    WriteU8U8Callback storeRAMStatus8 = nullptr;
    ReadU8Callback loadIO8 = nullptr;
    WriteU8Callback storeIO8 = nullptr;

    u16& getPC() { return m_state.stack[m_state.SP]; }
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
    void FIM(u8 idx);
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
    void RDx(u8 charIdx);
    void SBM();
    void SRC(u8 idx);
    void STC();
    void SUB(u8 idx);
    void TCC();
    void TCS();
    void WMP();
    void WRM();
    void WRR();
    void WRx(u8 charIdx);
    void XCH(u8 idx);

    const Mode m_mode;
    State m_state;
};
