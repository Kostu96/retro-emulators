#include "invaders.hpp"

#include "shared/source/application.hpp"
#include "shared/source/imgui/debug_view.hpp"
#include "shared/source/imgui/disassembly_view.hpp"
#include "utils/disassembly_line.hpp"
#include "cpu8080/disasm8080.hpp"

#include <imgui.h>
#include <thread>

class InvadersApp :
    public Application
{
public:
    InvadersApp(Invaders& invaders) :
        Application{ {
                .windowTitle = "Space Invaders Emulator by Kostu96",
                .rendererWidth = SCREEN_WIDTH,
                .rendererHeight = SCREEN_HEIGHT,
                .scale = 2,
                .border = 10,
                .hasMenuBar = true
        } },
        m_invaders{ invaders },
        m_debugView{ m_isPaused },
        m_disassemblyView{ m_disassembly, 4 }  {
        
        m_debugView.stepCallback = [&]() {
            if (m_isPaused) {
                m_invaders.runUntilNextInstruction();
                updateDisassembly();
            }
        };

        m_debugView.cpuStatusCallback = [&]() {
            const auto& cpuStatus = m_invaders.getCPU().getState();
            ImGui::Text("CPU Registers");
            ImGui::Separator();
            ImGui::Text("PC: %04X", cpuStatus.PC); ImGui::SameLine();
            ImGui::Text("SP: %04X", cpuStatus.SP); ImGui::SameLine();
            ImGui::Text("A: %02X", cpuStatus.A); ImGui::SameLine();
            ImGui::Text("F: %02X", cpuStatus.F.byte);
            ImGui::Text("BC: %04X", cpuStatus.BC); ImGui::SameLine();
            ImGui::Text("DE: %04X", cpuStatus.DE); ImGui::SameLine();
            ImGui::Text("HL: %04X", cpuStatus.HL);

            ImGui::SeparatorText("Instruction Trace");
            for (size_t i = 0; i < INSTRUCTION_TRACE_CAPACITY; i++) {
                ImGui::Text("0x%04X: %.*s", m_intructionTrace[i].address, static_cast<int>(DisassemblyLine::BUFFER_SIZE), m_intructionTrace[i].buffer);
            }
        };

        updateDisassembly();
    }

    void updateDisassembly() {
        u16 pc = m_invaders.getCPU().getState().PC;
        u8 opcode = m_invaders.memoryRead(pc);
        u8 byte1 = m_invaders.memoryRead(pc + 1);
        u8 byte2 = m_invaders.memoryRead(pc + 2);
        DisassemblyLine line;
        disasmIntruction(opcode, byte1, byte2, line);
        line.address = pc;

        for (size_t i = INSTRUCTION_TRACE_CAPACITY - 1; i > 0; i--) {
            m_intructionTrace[i] = m_intructionTrace[i - 1];
        }
        m_intructionTrace[0] = line;

        auto i = m_disassembly.begin();
        for (; i != m_disassembly.end(); i++)
            if (i->address >= line.address)
                break;

        if (m_disassembly.empty() || i == m_disassembly.end()) {
            m_disassembly.push_back(line);
        }
        else if (line.address == i->address) *i = line;
        else m_disassembly.insert(i, line);
    }

    bool isPaused() const { return m_isPaused; }
private:
    std::span<const unsigned int> getScreenPixels() const override { return m_invaders.getVideo().getScreenPixels(); }

    void onImGUIRender() override {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Emulator"))
        {
            ImGui::MenuItem("Start");
            if (ImGui::MenuItem("Reset")) m_invaders.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) exit();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Debug Control&Status", nullptr, &m_debugView.open);
            ImGui::MenuItem("Disassembly", nullptr, &m_disassemblyView.open);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        m_debugView.updateWindow();
        m_disassemblyView.updateWindow(m_invaders.getCPU().getState().PC);
    }

    Disassembly m_disassembly; // TODO(Kostu): move this inside DisassemblyView
    Invaders& m_invaders;
    bool m_isPaused = false;
    imgui::DebugView m_debugView;
    imgui::DisassemblyView m_disassemblyView;
    static constexpr size_t INSTRUCTION_TRACE_CAPACITY = 8;
    DisassemblyLine m_intructionTrace[INSTRUCTION_TRACE_CAPACITY];
};

int main()
{
    std::unique_ptr<Invaders> invaders = std::make_unique<Invaders>();
    InvadersApp app{ *invaders.get() };

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 10 }); // TODO: temp
                if (!app.isPaused()) {
                    invaders->clock();
                    if (invaders->getCPU().getCyclesLeft() == 0) {
                        app.updateDisassembly();
                    }
                }
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
