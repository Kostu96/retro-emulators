#pragma once
#include "shared/source/disassembly_line.hpp"
#include "shared/source/types.hpp"

#include <functional>

#if defined(PSX_TESTS)
#define PRIVATE public
#else
#define PRIVATE private
#endif

namespace PSX {

    class CPU
    {
    public:
        using Read8MemoryCallback = std::function<u8(u32)>;
        using Read32MemoryCallback = std::function<u32(u32)>;
        using Write8MemoryCallback = std::function<void(u32, u8)>;
        using Write16MemoryCallback = std::function<void(u32, u16)>;
        using Write32MemoryCallback = std::function<void(u32, u32)>;
        void mapRead8MemoryCallback(Read8MemoryCallback callback) { load8 = callback; }
        void mapRead32MemoryCallback(Read32MemoryCallback callback) { load32 = callback; }
        void mapWrite8MemoryCallback(Write8MemoryCallback callback) { store8 = callback; }
        void mapWrite16MemoryCallback(Write16MemoryCallback callback) { store16 = callback; }
        void mapWrite32MemoryCallback(Write32MemoryCallback callback) { store32 = callback; }

        static constexpr size_t CPU_REGISTER_COUNT = 32;
        static constexpr size_t COP0_REGISTER_COUNT = 16;

        struct CPUStatus {
            u32 regs[CPU_REGISTER_COUNT];
            u32 PC;
            u32 HI;
            u32 LO;
        };
        static_assert(sizeof(CPUStatus) == (CPU_REGISTER_COUNT + 3) * 4);

        union COP0Status {
            union StatusRegister {
                struct {
                    u8 IEKUStack : 6; // 0-5 - Interrupt Enable and Kernel/User Mode stack
                    u8 other0 : 2;    // 6-7
                    u8 other1;        // 8-15
                    u16 Isc : 1;      // 16 - Isolate Cache
                    u16 other2 : 5;   // 17-21
                    u16 BEV : 1;      // 22 - Boot exception vectors in RAM/ROM
                    u16 other3 : 9;   // 23-31
                };
                u32 value;
            };
            static_assert(sizeof(StatusRegister) == 4);

            struct {
                u32 regs0_2[3];    // 0-2
                u32 BPC;           // 3 - PC breakpoint
                u32 reg4;          // 4
                u32 BDA;           // 5 - Data breakpoint
                u32 reg6;          // 6
                u32 DCIC;          // 7
                u32 reg8;          // 8
                u32 BDAM;          // 9 - Data breakpoint mask
                u32 reg10;         // 10
                u32 BPCM;          // 11 - PC breakpoint mask
                StatusRegister SR; // 12 - Status register TODO: separate into flags
                u32 CAUSE;         // 13 - Interrupt cause
                u32 EPC;           // 14 - Return address from exception
                u32 reg15;         // 15

            };
            u32 regs[COP0_REGISTER_COUNT];
        };
        static_assert(sizeof(COP0Status) == COP0_REGISTER_COUNT * 4);

        void reset();
        void clock(DisassemblyLine& disasmLine);

        const CPUStatus& getCPUStatus() const { return m_cpuStatus; }
        const COP0Status& getCOP0Status() const { return m_cop0Status; }

        void overrideCPURegister(size_t index, u32 value) { m_cpuStatus.regs[index] = value; }

        CPU();
        CPU(CPU&) = delete;
        CPU& operator=(CPU&) = delete;
    PRIVATE:
        struct RegIndex
        {
            u8 i = 0;

            RegIndex() = default;
            explicit RegIndex(u8 index) : i{ index } {}
        };

        struct Instruction
        {
            u32 word;

            Instruction(u32 _word) : word(_word) {}

            u32 opcode() const { return word >> 26; }
            u32 subfn() const { return word & 0x3F; }
            u32 copfn() const { return (word >> 21) & 0x1F; }
            RegIndex regS() const { return RegIndex{ (word >> 21) & 0x1F }; }
            RegIndex regT() const { return RegIndex{ (word >> 16) & 0x1F }; }
            RegIndex regD() const { return RegIndex{ (word >> 11) & 0x1F }; }
            u32 imm() const { return word & 0xFFFF; }
            u32 imm_se() const { return static_cast<s16>(word & 0xFFFF); }
            u32 imm_se_jump() const { return static_cast<s16>(word & 0xFFFF) << 2; }
            u32 imm_jump() const { return (word & 0x3FFFFFF) << 2; }
            u32 shift() const { return (word >> 6) & 0x1F; }
        };

        enum class Exception {
            LoadAddressError = 0x4,
            StoreAddressError = 0x5,
            SysCall = 0x8,
            Break = 0x9,
            IllegallInstruction = 0xA,
            CoprocessorError = 0xB,
            Overflow = 0xC
        };

        struct PendingLoad
        {
            RegIndex regIndex{};
            u32 value = 0;
        };

        Read8MemoryCallback load8 = nullptr;
        Read32MemoryCallback load32 = nullptr;
        Write8MemoryCallback store8 = nullptr;
        Write16MemoryCallback store16 = nullptr;
        Write32MemoryCallback store32 = nullptr;
        void setReg(RegIndex index, u32 value);
        u32 getReg(RegIndex index) const;
        void exception(Exception cause);
        void branch(bool condition, u32 offset);

        void op_MFC0(RegIndex copIndex, RegIndex cpuIndex);
        void op_MTC0(RegIndex copIndex, RegIndex cpuIndex);
        void op_RFE();

        void op_SYSCALL();
        void op_BXX(RegIndex s, u32 offset, u32 opcode);
        void op_SLL(RegIndex d, RegIndex t, u32 shift);
        void op_SRL(RegIndex d, RegIndex t, u32 shift);
        void op_SRA(RegIndex d, RegIndex t, u32 shift);
        void op_OR(RegIndex target, RegIndex lhs, u32 rhs);
        void op_AND(RegIndex target, RegIndex lhs, u32 rhs);
        void op_J(u32 immediate);
        void op_JAL(u32 immediate);
        void op_JR(RegIndex s);
        void op_JALR(RegIndex d, RegIndex s);
        void op_ADD(RegIndex d, RegIndex s, u32 rhs);
        void op_ADDU(RegIndex d, RegIndex s, u32 rhs);
        void op_SUBU(RegIndex d, RegIndex s, u32 rhs);
        void op_LUI(RegIndex t, u32 immediate);
        void op_SB(RegIndex t, RegIndex s, u32 immediate);
        void op_SH(RegIndex t, RegIndex s, u32 immediate);
        void op_SW(RegIndex t, RegIndex s, u32 immediate);
        void op_LB(RegIndex t, RegIndex s, u32 immediate);
        void op_LBU(RegIndex t, RegIndex s, u32 immediate);
        void op_LW(RegIndex t, RegIndex s, u32 immediate);
        void op_SLTU(RegIndex d, u32 lhs, u32 rhs);
        void op_SLT(RegIndex t, s32 lhs, s32 rhs);
        void op_DIV(s32 numerator, s32 denominator);
        void op_DIVU(u32 numerator, u32 denominator);
        void op_MFHI(RegIndex d);
        void op_MTHI(RegIndex s);
        void op_MFLO(RegIndex d);
        void op_MTLO(RegIndex s);

        CPUStatus m_cpuStatus;
        COP0Status m_cop0Status;

        PendingLoad m_pendingLoad{};
        u32 m_helperCPURegs[CPU_REGISTER_COUNT];
        u32 m_currentPC;
        u32 m_nextPC;
        bool m_isBranch;
        bool m_isBranchDelaySlot;

        friend void disasm(u32 address, CPU::Instruction opcode, DisassemblyLine& output); // TODO: temp
    };

} // namespace PSX
