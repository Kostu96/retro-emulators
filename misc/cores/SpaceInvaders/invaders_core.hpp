#pragma once
#include "../../shared/emulator_core.hpp"
#include "cpu_x80.hpp"
#include "disasm8080.hpp"

class InvadersCore :
    public EmulatorCore
{
public:
    const EmulatorSettings& getEmulatorSettings() const override { return m_emulatorSettings; }

    const size_t* getMemorySizes() const override { return m_memorySizes; }
    const int* getMemoryColsNumbers() const override { return m_memoryColsNumbers; }
    const std::vector<DisassemblyLine>& getDisassembly() const override { return m_disassembly; }
    const std::vector<StateEntry>& getState() const override { return m_state; }
    
    u8 getByteAt(u16 address, size_t memoryIndex) const override;
    u16 getPC() const override { return m_cpu.getPC(); }

    void handleKey(int key, int action) override;

    void loadROM(const char* filename) override {}
    void reset() override;
    void update(double dt) override;
    
    void setRenderPointCallback(RenderPointCallback callback) override { m_renderPoint = callback; }

    InvadersCore();
private:
    void updateState();

    const size_t m_memorySizes[1] = { 0x4000 };
    const int m_memoryColsNumbers[1] = { 16 };

    CPUx80<CPUx80Mode::Intel8080> m_cpu;

    const EmulatorSettings m_emulatorSettings;
    std::vector<DisassemblyLine> m_disassembly;
    std::vector<StateEntry> m_state;

    RenderPointCallback m_renderPoint = nullptr;
};
