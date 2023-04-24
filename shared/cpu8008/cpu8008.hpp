#pragma once
#include <ccl/non_copyable.h>
#include <ccl/types.hpp>

#include <vector>

class CPU8008 :
    public ccl::NonCopyable
{
public:
union Flags
    {
        struct {
            u8 C      : 1; // 0
            u8 P      : 1; // 1
            u8 S      : 1; // 2
            u8 Z      : 1; // 3
            u8 unused : 4; // 4-7
        };
        u8 byte;
    };

    void reset();
    void clock();

    u8 load8(u16 address) const;

    CPU8008() = default;
private:
    u16 load16(u16 address);
    void store8(u16 address, u8 data);
    void store16(u16 address, u16 data);
    void push8(u8 data);
    void push16(u16 data);
    u8 pop8();
    u16 pop16();

    u16 m_stack[8];
    u8 SP : 3;
    u8 A;
    u8 B;
    u8 C;
    u8 D;
    u8 E;
    u8 H;
    u8 L;
    Flags F;
};
