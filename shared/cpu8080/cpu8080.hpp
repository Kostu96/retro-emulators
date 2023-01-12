#pragma once
#include "../memory_map.hpp"

#include <vector>

class CPU8080
{
public:
    enum class Mode
    {
        Intel8080,
        Z80,
        GameBoy
    };

    union Flags
    {
        struct {
            u8 C       : 1; // 0
            u8 unused1 : 1; // 1
            u8 P       : 1; // 2
            u8 unused3 : 1; // 3
            u8 AC      : 1; // 4
            u8 unused5 : 1; // 5
            u8 S       : 1; // 6
            u8 Z       : 1; // 7
        } bits;
        u8 byte;
    };

    template <Mapable Device>
    void map(Device& device, AddressRange range);
    template <ConstMapable ConstDevice>
    void map(const ConstDevice& device, AddressRange range);

    void reset();
    void clock();

    u8 load8(u16 address) const;
    u16 getAF() const { return AF; }
    u16 getBC() const { return BC; }
    u16 getDE() const { return DE; }
    u16 getHL() const { return HL; }
    u16 getSP() const { return SP; }
    u16 getPC() const { return PC; }

    explicit CPU8080(Mode mode) : m_mode{ mode } {}
    CPU8080(const CPU8080&) = delete;
    CPU8080& operator=(const CPU8080&) = delete;
private:
    u16 load16(u16 address);
    void store8(u16 address, u8 data);
    void store16(u16 address, u16 data);
    void push8(u8 data);
    void push16(u16 data);
    u8 pop8();
    u16 pop16();

    // Data transfer group
    void LDR(u8& dst, u8 value);
    void LDRP(u16& dst, u16 value);
    void LDM(u16 address, u8 value);
    void XCH();

    // Arithmetic group
    void ADD(u8 value);
    void ADDHL(u16 value);
    void CMP(u8 value);
    void DECR(u8& reg);
    void DECRP(u16& reg);
    void INCR(u8& reg);
    void INCRP(u16& reg);
    void RRC();

    // Logical group
    void AND(u8 value);
    void XOR(u8 value);

    // Branch group
    void JMP(bool flag);
    void CALL(bool flag);
    void RET(bool flag);

    // CPU State:
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
            u8 B, C;
        };
        u16 BC;
    };
    union {
        struct {
            u8 D, E;
        };
        u16 DE;
    };
    union {
        struct {
            u8 H, L;
        };
        u16 HL;
    };

    // Helpers:
    Mode m_mode;
    std::vector<ReadMapEntry> m_readMap;
    std::vector<WriteMapEntry> m_writeMap;
};

template<Mapable Device>
inline void CPU8080::map(Device& device, AddressRange range)
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
inline void CPU8080::map(const ConstDevice& device, AddressRange range)
{
    m_readMap.emplace_back(
        ReadMapEntry{
            range,
            [&device](u16 address) { return device.read(address); }
        }
    );
}
