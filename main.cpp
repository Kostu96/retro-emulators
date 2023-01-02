#include "dll_loader.hpp"
#include "emulator_core.hpp"
#include "shader_sources.inl"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

#pragma region OGLStuff
glw::Shader* pointShader = nullptr;
struct CharVertex {
    u16 x, y;
    u32 color;
} charVertices[42];
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

   /* for (u16 row = 0; row < CHIP8_HEIGHT; row++)
        for (u16 col = 0; col < CHIP8_WIDTH; col++)
        {
            charVertices[row * CHIP8_WIDTH + col].x = col;
            charVertices[row * CHIP8_WIDTH + col].y = row;
            charVertices[row * CHIP8_WIDTH + col].color = 0;
        }*/

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

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        DllLoader<EmulatorCore> loader{ argv[1] };
        
        if (!loader.openLib()) {
           std::cerr << "Cannot open library: " << argv[1] << '\n';
           std::terminate();
        }

        auto core = loader.getInstance();

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
        glfwMakeContextCurrent(window);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        glfwTerminate();
        loader.closeLib();
    }
    else
    {
        std::cerr << "Invalid number of arguments.\n";
    }

    return 0;
}
