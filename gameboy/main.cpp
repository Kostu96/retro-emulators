#include "gameboy.hpp"
#include "gui.hpp"
#include "timer.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>

constexpr u16 FRAME_WIDTH = 160;
constexpr u16 FRAME_HEIGHT = 144;
constexpr u16 SCALE = 3;
constexpr u16 BORDER_SIZE = 10;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 16;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

int main(int /*argc*/, char* /*argv*/[])
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_BAR_HEIGHT, "Game Boy Emulator | Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.9f, 0.9f, 0.9f, 1.f);

    GUI::init(window);

    Gameboy gameboy;

    //std::string path{ "C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/" };
    std::string path{ "C:/Users/kmisiak/myplace/retro-extras/programs/gameboy/" };
    //path += "Blargg_tests/01-special.gb";            // +
    //path += "Blargg_tests/02-interrupts.gb";         // +
    //path += "Blargg_tests/03-op sp,hl.gb";           // +
    //path += "Blargg_tests/04-op r,imm.gb";           // +
    //path += "Blargg_tests/05-op rp.gb";              // +
    //path += "Blargg_tests/06-ld r,r.gb";             // +
    //path += "Blargg_tests/07-jr,jp,call,ret,rst.gb"; // +
    //path += "Blargg_tests/08-misc instrs.gb";        // +
    //path += "Blargg_tests/09-op r,r.gb";             // +
    //path += "Blargg_tests/10-bit ops.gb";            // +
    //path += "Blargg_tests/11-op a,(hl).gb";          // +
    //path += "Blargg_tests/cpu_instrs.gb";
    //path += "Blargg_tests/instr_timing.gb";          // +
    //path += "Blargg_tests/mem_timing.gb";
    path += "dmg-acid2.gb";
    //path += "tetris.gb";
    gameboy.loadCartridge(path.c_str());

    /*std::ofstream log{ "dr_log.txt " };
    assert(log.is_open());
    log << std::setfill('0') << std::uppercase;*/

    while (!glfwWindowShouldClose(window))
    {
        int repeats = 256;
        while (repeats--)
        {
            gameboy.update();
            /*if (!mapBootloader && cpu.getCyclesLeft() == 0 && false) {
                u16 PC = cpu.getPC();
                log << "A:" << std::setw(2) << std::hex << (cpu.getAF() >> 8);
                log << " F:" << std::setw(2) << std::hex << (cpu.getAF() & 0xFF);
                log << " B:" << std::setw(2) << std::hex << (cpu.getBC() >> 8);
                log << " C:" << std::setw(2) << std::hex << (cpu.getBC() & 0xFF);
                log << " D:" << std::setw(2) << std::hex << (cpu.getDE() >> 8);
                log << " E:" << std::setw(2) << std::hex << (cpu.getDE() & 0xFF);
                log << " H:" << std::setw(2) << std::hex << (cpu.getHL() >> 8);
                log << " L:" << std::setw(2) << std::hex << (cpu.getHL() & 0xFF);
                log << " SP:" << std::setw(4) << std::hex << cpu.getSP();
                log << " PC:" << std::setw(4) << std::hex << PC;
                log << " PCMEM:" << std::setw(2) << std::hex << (u16)readCallback(PC) << ',';
                log << std::setw(2) << std::hex << (u16)readCallback(PC + 1) << ',';
                log << std::setw(2) << std::hex << (u16)readCallback(PC + 2) << ',';
                log << std::setw(2) << std::hex << (u16)readCallback(PC + 3);
                log << '\n';
            }*/
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(BORDER_SIZE, BORDER_SIZE, WINDOW_WIDTH - 2 * BORDER_SIZE, WINDOW_HEIGHT - 2 * BORDER_SIZE);
        gameboy.getScreenTexture()->bind(0);
        float left = -1.f;
        float right = 1.f;
        float top = 1.f;
        float bottom = -1.f;
        glw::Renderer::renderTexture(left, top, right, bottom, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();

        GUI::update(gameboy);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //log.close();

    GUI::shutdown();
    glw::Renderer::shutdown();
    glfwTerminate();
    return 0;
}
