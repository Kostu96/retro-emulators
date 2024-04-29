#pragma once
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

        static constexpr size_t REGISTER_COUNT = 32;

        struct CPUStatus {
            u32 PC;
            u32 inputRegs[REGISTER_COUNT];
            u32 outputRegs[REGISTER_COUNT];
        };

        union COP0Status {
            struct {
                u32 regs0_11[12];  // 0-11
                u32 SR;            // 12 TODO: separate into flags
                u32 regs13_31[19]; // 13-31

            };
            u32 regs[REGISTER_COUNT];
        };

        void reset();
        void clock();

        const CPUStatus& getCPUStatus() const { return m_cpuStatus; }
        const COP0Status& getCOP0Status() const { return m_cop0Status; }

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
            u32 imm_jump() const { return (word & 0x3FFFFFF) << 2; }
            u32 shift() const { return (word >> 6) & 0x1F; }
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
        u32 getReg(RegIndex index) const { return m_cpuStatus.inputRegs[index.i]; }
        void branch(bool condition, u32 offset);

        void op_MTC0(RegIndex copIndex, RegIndex cpuIndex);

        void op_SLL(RegIndex d, RegIndex t, u32 shift);
        void op_OR(RegIndex target, RegIndex lhs, u32 rhs);
        void op_AND(RegIndex target, RegIndex lhs, u32 rhs);
        void op_J(u32 immediate);
        void op_JAL(u32 immediate);
        void op_ADDI(RegIndex t, RegIndex s, u32 immediate);
        void op_ADDU(RegIndex t, RegIndex s, u32 rhs);
        void op_LUI(RegIndex t, u32 immediate);
        void op_SB(RegIndex t, RegIndex s, u32 immediate);
        void op_SH(RegIndex t, RegIndex s, u32 immediate);
        void op_SW(RegIndex t, RegIndex s, u32 immediate);
        void op_LW(RegIndex t, RegIndex s, u32 immediate);
        void op_SLTU(RegIndex d, u32 lhs, u32 rhs);

        CPUStatus m_cpuStatus;
        COP0Status m_cop0Status;

        Instruction m_nextInstruction = 0;
        PendingLoad m_pendingLoad{};
    };

} // namespace PSX
