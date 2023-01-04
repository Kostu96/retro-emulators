#include "dll_loader.hpp"
#include "emulator_core.hpp"
#include "shader_sources.inl"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

#pragma region OGLStuff
glw::Shader* pointShader = nullptr;
CharVertex charVertices[64*32];
glw::VertexBuffer* charVBO = nullptr;
glw::VertexArray* charVAO = nullptr;

glw::Shader* textureShader = nullptr;
struct TextureVertex {
    f32 x, y;
    f32 u, v;
} textureVertices[] = {
    -1.f, -1.f, 0.f, 0.f,
     1.f, -1.0f, 1.f, 0.f,
     1.f,  1.f, 1.f, 1.f,
    -1.f,  1.f, 0.f, 1.f
};
u8 textureIndices[] = {
    0, 1, 2,
    2, 3, 0
};
glw::VertexBuffer* textureVBO = nullptr;
glw::IndexBuffer* textureIBO = nullptr;
glw::VertexArray* textureVAO = nullptr;

glw::Framebuffer* FBO = nullptr;

void init(u16 fbWidth, u16 fbHeight)
{
    glActiveTexture(GL_TEXTURE0);

    pointShader = new glw::Shader{};
    pointShader->createFromSource(pointVertSource, pointFragSource);
    textureShader = new glw::Shader{};
    textureShader->createFromSource(textureVertSource, textureFragSource);

    charVBO = new glw::VertexBuffer{ sizeof(charVertices) };
    glw::VertexLayout pointLayout{ {
        { glw::LayoutElement::DataType::U16_2, false },
        { glw::LayoutElement::DataType::U8_4, true, true }
    } };
    charVAO = new glw::VertexArray{ *charVBO, pointLayout };

    textureVBO = new glw::VertexBuffer{ textureVertices, sizeof(textureVertices) };
    textureIBO = new glw::IndexBuffer{ textureIndices, sizeof(textureIndices) / sizeof(u8), glw::IndexBuffer::IndexType::U8 };
    glw::VertexLayout texturelayout{ {
        { glw::LayoutElement::DataType::F32_2 },
        { glw::LayoutElement::DataType::F32_2 }
    } };
    textureVAO = new glw::VertexArray{ *textureVBO, texturelayout };
    textureVAO->setIndexBuffer(*textureIBO);

    for (u16 row = 0; row < 32; row++)
        for (u16 col = 0; col < 64; col++)
        {
            charVertices[row * 64 + col].x = col;
            charVertices[row * 64 + col].y = row;
            charVertices[row * 64 + col].color = 0;
        }

    FBO = new glw::Framebuffer{
        glw::Framebuffer::Properties{ fbWidth, fbHeight, 1, {
            glw::TextureSpecification{
                glw::TextureFormat::RGBA8,
                glw::TextureFilter::Nearest,
                glw::TextureFilter::Nearest,
                glw::TextureWrapMode::Clamp
            }
        }}
    };
}

void shutdown()
{
    delete FBO;

    delete textureVAO;
    delete textureIBO;
    delete textureVBO;

    delete charVAO;
    delete charVBO;

    delete textureShader;
    delete pointShader;
}
#pragma endregion

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    auto core = reinterpret_cast<EmulatorCore*>(glfwGetWindowUserPointer(window));
    core->handleKey(key, action);
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        DllLoader<EmulatorCore> loader{ argv[1] };
        if (!loader.openLib()) {
           std::cerr << "Cannot open library: " << argv[1] << '\n';
           std::terminate();
        }

        {
            auto core = loader.getInstance();
            core->loadROM("roms/chip8/chip8-test-suite.ch8");
            core->reset();

            glfwSetErrorCallback(glfwErrorCallback);
            if (!glfwInit()) {
                std::cerr << "GLFW init failed!\n";
                std::terminate();
            }

            EmulatorCore::WindowSettings settings;
            core->getWindowSettings(settings);

            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
            GLFWwindow* window = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
            if (!window) {
                std::cerr << "GLFW window creation failed!\n";
                std::terminate();
            }
            glfwSetKeyCallback(window, glfwKeyCallback);
            glfwSetWindowUserPointer(window, core.get());
            glfwMakeContextCurrent(window);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 450");

            glw::init(glfwGetProcAddress);
            init(64, 32);

            while (!glfwWindowShouldClose(window))
            {
                glfwPollEvents();

                core->clock();
                core->clock();

                FBO->bind();
                charVAO->bind();
                pointShader->bind();
                glViewport(0, 0, 64, 32);
                core->render(charVertices);
                charVBO->bind();
                charVBO->setData(charVertices, sizeof(charVertices));
                glDrawArrays(GL_POINTS, 0, 64 * 32);

                FBO->unbind();
                textureVAO->bind();
                textureShader->bind();
                glViewport(0, 0, settings.width, settings.height);
                FBO->getAttachments()[0].bind(0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
                glFinish();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                {
                    ImGui::Begin("Memory");
                    auto mem = core->getMemory();
                    if (ImGui::BeginTable("memory_table", 9))
                    {
                        u16 addr = 0;
                        for (u8 byte : mem)
                        {
                            if (addr % 8 == 0)
                            {
                                ImGui::TableNextColumn();
                                ImGui::Text("%03X:", addr);
                            }

                            ImGui::TableNextColumn();
                            ImGui::Text("%02X", byte);
                            addr++;
                        }

                        ImGui::EndTable();
                    }

                    ImGui::End();
                }

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }

                glfwSwapBuffers(window);
            }

            shutdown();
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwTerminate();

        }
        loader.closeLib();
    }
    else
    {
        std::cerr << "Invalid number of arguments.\n";
    }

    return 0;
}
