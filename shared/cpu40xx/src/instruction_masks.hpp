#pragma once
#include "utils/types.hpp"

namespace CPU40xxInstructionHighNibbleMasks {

constexpr u8 NOP              = 0x0;
constexpr u8 JCN              = 0x1;
constexpr u8 FIMorSRC         = 0x2;
constexpr u8 FINorJIN         = 0x3;
constexpr u8 JUN              = 0x4;
constexpr u8 JMS              = 0x5;
constexpr u8 INC              = 0x6;
constexpr u8 ISZ              = 0x7;
constexpr u8 ADD              = 0x8;
constexpr u8 SUB              = 0x9;
constexpr u8 LD               = 0xA;
constexpr u8 XCH              = 0xB;
constexpr u8 BBL              = 0xC;
constexpr u8 LDM              = 0xD;
constexpr u8 MemoryGroup      = 0xE;
constexpr u8 AccumulatorGroup = 0xF;

} // namespace CPU40xxInstructionHighNibbleMasks

namespace CPU40xxInstructionLowNibbleMasks {

constexpr u8 WRM = 0x0;
constexpr u8 WMP = 0x1;
constexpr u8 WRR = 0x2;
constexpr u8 WPM = 0x3;
constexpr u8 WR0 = 0x4;
constexpr u8 WR1 = 0x5;
constexpr u8 WR2 = 0x6;
constexpr u8 WR3 = 0x7;
constexpr u8 SBM = 0x8;
constexpr u8 RDM = 0x9;
constexpr u8 RDR = 0xA;
constexpr u8 ADM = 0xB;
constexpr u8 RD0 = 0xC;
constexpr u8 RD1 = 0xD;
constexpr u8 RD2 = 0xE;
constexpr u8 RD3 = 0xF;

constexpr u8 CLB = 0x0;
constexpr u8 CLC = 0x1;
constexpr u8 IAC = 0x2;
constexpr u8 CMC = 0x3;
constexpr u8 CMA = 0x4;
constexpr u8 RAL = 0x5;
constexpr u8 RAR = 0x6;
constexpr u8 TCC = 0x7;
constexpr u8 DAC = 0x8;
constexpr u8 TCS = 0x9;
constexpr u8 STC = 0xA;
constexpr u8 DAA = 0xB;
constexpr u8 KBP = 0xC;
constexpr u8 DCL = 0xD;

} // namespace CPU40xxInstructionLowNibbleMasks
