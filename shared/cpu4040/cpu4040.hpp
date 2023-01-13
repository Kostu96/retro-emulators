#pragma once
#include "../memory_map.hpp"

class CPU4040
{
public:
    enum class Mode : u8
    {
        Intel4004,
        Intel4040
    };

    template <ConstMapable ConstDevice>
    void map(const ConstDevice& device, AddressRange range);

    void reset();
    void clock();

    const u8* getRegs() const { return m_regs; }
    const u16* getStack() const { return m_stack; }
    u8 getSP() const { return SP; }
    u8 getACC() const { return ACC; }
    u8 getCY() const { return CY; }

    explicit CPU4040(Mode mode);
    ~CPU4040();
    CPU4040(const CPU4040&) = delete;
    CPU4040& operator=(const CPU4040&) = delete;
private:
    void incPC() { m_stack[SP]++; }
    u16 getPC() const { return m_stack[SP] % 0xFFF; }
    u8 load8(u16 address) const;

    void ADD(u8* regs, u8 idx);
    void FIM(u8* reg);
    void LD(u8* regs, u8 idx);

    const Mode m_mode;
    const u8 REGS_SIZE;
    const u8 STACK_SIZE;
    
    u8* m_regs;
    u16* m_stack;
    u8 SP;
    u8 ACC;
    u8 CY;

    std::vector<ReadMapEntry> m_readMap;
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
