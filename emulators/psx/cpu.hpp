#pragma once
#include "shared/source/types.hpp"

#include <functional>

namespace PSX {

    class CPU
    {
    public:
        using Read8MemoryCallback = std::function<u8(u32)>;
        using Read32MemoryCallback = std::function<u32(u32)>;
        using Write8MemoryCallback = std::function<void(u32, u8)>;
        using Write32MemoryCallback = std::function<void(u32, u32)>;
        void mapRead8MemoryCallback(Read8MemoryCallback callback) { load8 = callback; }
        void mapRead32MemoryCallback(Read32MemoryCallback callback) { load32 = callback; }
        void mapWrite8MemoryCallback(Write8MemoryCallback callback) { store8 = callback; }
        void mapWrite32MemoryCallback(Write32MemoryCallback callback) { store32 = callback; }

        struct CPUStatus {
            u32 PC;
            u32 regs[32];
        };

        union COP0Status {
            struct {
                u32 regs0_11[12];  // 0-11
                u32 SR;            // 12
                u32 regs13_31[19]; // 13-31

            };
            u32 regs[32];
        };

        void reset();
        void clock();

        const CPUStatus& getCPUStatus() const { return m_cpuStatus; }
        const COP0Status& getCOP0Status() const { return m_cop0Status; }

        CPU();
        CPU(CPU&) = delete;
        CPU& operator=(CPU&) = delete;
    private:
        struct Instruction
        {
            u32 word;

            Instruction(u32 _word) : word(_word) {}

            u32 opcode() const { return word >> 26; }
            u32 subfn() const { return word & 0x3F; }
            u32 copfn() const { return (word >> 21) & 0x1F; }
            u32 regS() const { return (word >> 21) & 0x1F; }
            u32 regT() const { return (word >> 16) & 0x1F; }
            u32 regD() const { return (word >> 11) & 0x1F; }
            u32 imm() const { return word & 0xFFFF; }
            u32 imm_se() const { return static_cast<s16>(word & 0xFFFF); }
            u32 imm_jump() const { return (word & 0x3FFFFFF) << 2; }
            u32 shift() const { return (word >> 6) & 0x1F; }
        };

        Read8MemoryCallback load8 = nullptr;
        Read32MemoryCallback load32 = nullptr;
        Write8MemoryCallback store8 = nullptr;
        Write32MemoryCallback store32 = nullptr;
        void setReg(u32 index, u32 value);
        void branch(bool condition, u32 offset);

        void op_MTC0(u32 copIndex, u32 cpuIndex);

        void op_SLL(u32 dIndex, u32 tIndex, u32 shift);
        void op_OR(u32 targetIndex, u32 lhsIndex, u32 rhsValue);
        void op_J(u32 immediate);
        void op_ADDI(u32 tIndex, u32 sIndex, u32 immediate);
        void op_ADDIU(u32 tIndex, u32 sIndex, u32 immediate);
        void op_LUI(u32 index, u32 immediate);
        void op_SW(u32 tIndex, u32 sIndex, u32 immediate);

        CPUStatus m_cpuStatus;
        COP0Status m_cop0Status;
    };

} // namespace PSX
