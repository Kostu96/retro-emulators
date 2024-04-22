#include "gb_doctor.hpp"

#define GB_DOCTOR_LOG 0

#if GB_DOCTOR_LOG == 1
#include <cassert>
#include <fstream>
#include <iomanip>

static std::ofstream s_log;
#endif

void initGBDoctor()
{
#if GB_DOCTOR_LOG == 1
    s_log.open("gbdoctor.log");
    assert(s_log.is_open());
    s_log << std::setfill('0') << std::uppercase;
#endif
}

void shutdownGBDoctor()
{
#if GB_DOCTOR_LOG == 1
    s_log.close();
#endif
}

void updateGBDoctor(bool isBootloaderUnmapped, bool isLastCycle, const CPU::State& cpuState, CPU::ReadMemoryCallback readMemory)
{
#if GB_DOCTOR_LOG == 1
    if (isBootloaderUnmapped && isLastCycle) {
        s_log << "A:" << std::setw(2) << std::hex << (u16)cpuState.A;
        s_log << " F:" << std::setw(2) << std::hex << (u16)cpuState.F.byte;
        s_log << " B:" << std::setw(2) << std::hex << (u16)cpuState.B;
        s_log << " C:" << std::setw(2) << std::hex << (u16)cpuState.C;
        s_log << " D:" << std::setw(2) << std::hex << (u16)cpuState.D;
        s_log << " E:" << std::setw(2) << std::hex << (u16)cpuState.E;
        s_log << " H:" << std::setw(2) << std::hex << (u16)cpuState.H;
        s_log << " L:" << std::setw(2) << std::hex << (u16)cpuState.L;
        s_log << " SP:" << std::setw(4) << std::hex << cpuState.SP;
        s_log << " PC:" << std::setw(4) << std::hex << cpuState.PC;
        s_log << " PCMEM:" << std::setw(2) << std::hex << (u16)readMemory(cpuState.PC) << ',';
        s_log << std::setw(2) << std::hex << (u16)readMemory(cpuState.PC + 1) << ',';
        s_log << std::setw(2) << std::hex << (u16)readMemory(cpuState.PC + 2) << ',';
        s_log << std::setw(2) << std::hex << (u16)readMemory(cpuState.PC + 3);
        s_log << '\n';
    }
#else
    (void)isBootloaderUnmapped;
    (void)isLastCycle;
    (void)cpuState;
    (void)readMemory;
#endif
}
