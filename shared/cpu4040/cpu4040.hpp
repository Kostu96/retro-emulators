#pragma once
#include "../memory_map.hpp"

class CPU4040
{
public:
    using ReadROMIOFunc = std::function<u8(u8)>;
    using WriteRAMOutFunc = std::function<void(u8, u8)>;

    enum class Mode : u8
    {
        Intel4004,
        Intel4040
    };

    template <ConstMapable ConstDevice>
    void map(const ConstDevice& device, AddressRange range);
    void mapReadROMIO(ReadROMIOFunc func) { m_readROMIO = func; }

    void reset();
    void clock();

    const u8* getRegs() const { return m_regs; }
    const u16* getStack() const { return m_stack; }
    u16 getPC() const { return m_stack[getSP()] % 0xFFF; }
    u8 getSP() const { return SP % STACK_SIZE; }
    u8 getACC() const { return ACC; }
    u8 getCY() const { return CY; }
    u8 getRAMAddr() const { return RAMAddr; }

    explicit CPU4040(Mode mode);
    ~CPU4040();
    CPU4040(const CPU4040&) = delete;
    CPU4040& operator=(const CPU4040&) = delete;
private:
    void incPC() { m_stack[getSP()]++; }
    u8 load8(u16 address) const;

    void ADD(const u8* regs, u8 idx);
    void BBL(u8 data);
    void CLB();
    void CLC();
    void DAC();
    void FIM(u8* reg);
    void INC(u8* regs, u8 idx);
    void JCM(u8 condition);
    void JMS(u16 highNibble);
    void JUN(u16 highNibble);
    void LD(const u8* regs, u8 idx);
    void LDM(u8 data);
    void RAL();
    void RAR();
    void RDR();
    void SRC(const u8* reg);
    void WMP();
    void XCH(u8* regs, u8 idx);

    const Mode m_mode;
    const u8 REGS_SIZE;
    const u8 STACK_SIZE;
    
    u8* m_regs;
    u16* m_stack;
    u8 SP;
    u8 ACC;
    u8 CY;
    u8 RAMAddr;

    std::vector<ReadMapEntry> m_readMap;
    ReadROMIOFunc m_readROMIO;
    WriteRAMOutFunc m_writeRAMOut;
};

template<ConstMapable ConstDevice>
inline void CPU4040::map(const ConstDevice& device, AddressRange range)
{
    m_readMap.emplace_back(
        ReadMapEntry{
            range,
            [&device](u16 address) { return device.read(address); }
        }
    );
}
