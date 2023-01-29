#pragma once
#include "../memory_map.hpp"

class CPU40xx
{
public:
    using ReadInputFunc = std::function<u8(u8)>;
    using WriteOutputFunc = std::function<void(u8, u8)>;
    using ReadRAMStatusFunc = std::function<u8(u8)>;
    using WriteRAMStatusFunc = std::function<void(u8, u8)>;

    enum class Mode : u8
    {
        Intel4004,
        Intel4040
    };

    template <Mapable Device>
    void map(Device& device, AddressRange range);
    template <ConstMapable ConstDevice>
    void map(const ConstDevice& device, AddressRange range);
    void mapReadROMIO(ReadInputFunc func) { m_readROMIO = func; }
    void mapWriteROMIO(WriteOutputFunc func) { m_writeROMIO = func; }
    void mapWriteRAMOut(WriteOutputFunc func) { m_writeRAMOut = func; }
    void mapReadRAMStatus(ReadRAMStatusFunc func) { m_readRAMStatus = func; }
    void mapWriteRAMStatus(WriteRAMStatusFunc func) { m_writeRAMStatus = func; }

    void reset();
    void clock();

    u8 loadROM8(u16 address) const;
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
    void incPC() { m_stack[getSP()]++; }
    u8 loadRAM4();
    void storeRAM4(u8 data);

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

    std::vector<ReadMapEntry> m_readMap;
    std::vector<WriteMapEntry> m_writeMap;
    ReadInputFunc m_readROMIO;
    WriteOutputFunc m_writeROMIO;
    WriteOutputFunc m_writeRAMOut;
    ReadRAMStatusFunc m_readRAMStatus;
    WriteRAMStatusFunc m_writeRAMStatus;
};

template<Mapable Device>
inline void CPU40xx::map(Device& device, AddressRange range)
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
inline void CPU40xx::map(const ConstDevice& device, AddressRange range)
{
    m_readMap.emplace_back(
        ReadMapEntry{
            range,
            [&device](u16 address) { return device.read(address); }
        }
    );
}
