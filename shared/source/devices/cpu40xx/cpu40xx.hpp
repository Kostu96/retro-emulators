#pragma once
#include "../../types.hpp"

#include <functional>

class CPU40xx
{
public:
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

    enum class Mode : u8
    {
        Intel4004,
        Intel4040
    };

    void reset();
    void clock();

    const u8* getRegs() const { return m_regs; }
    const u16* getStack() const { return m_stack; }
    u16 getPC() const { return m_stack[getSP()] % 0xFFF; }
    u8 getSP() const { return SP % STACK_SIZE; }
    u8 getACC() const { return ACC; }
    u8 getCY() const { return CY; }
    u8 getSRCReg() const { return SRCReg; }
    u8 getCMRAM() const { return CMRAM; }

    explicit CPU40xx(Mode mode);
    ~CPU40xx();
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

    u8 getRAMAddress() const { return (CMRAM >> 1) | SRCReg; }
    void incPC() { m_stack[getSP()]++; }

    void ADD(const u8* regs, u8 idx);
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
    void FIN(u8* reg);
    void IAC();
    void INC(u8* regs, u8 idx);
    void ISZ(u8* regs, u8 idx);
    void JCN(u8 condition);
    void JIN(const u8* reg);
    void JMS(u16 highNibble);
    void JUN(u16 highNibble);
    void KBP();
    void LD(const u8* regs, u8 idx);
    void LDM(u8 data);
    void RAL();
    void RAR();
    void RDM();
    void RDR();
    void RDX(u8 charIdx);
    void SBM();
    void SRC(const u8* reg);
    void STC();
    void SUB(const u8* regs, u8 idx);
    void TCC();
    void TCS();
    void WMP();
    void WRM();
    void WRR();
    void WRX(u8 charIdx);
    void XCH(u8* regs, u8 idx);

    const Mode m_mode;
    const u8 REGS_SIZE;
    const u8 STACK_SIZE;
    
    u8* m_regs;
    u16* m_stack;
    u8 SP : 3;
    u8 ACC : 4;
    u8 CY : 1;
    u8 m_test : 1;
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
    u8 CMRAM : 4;
};
