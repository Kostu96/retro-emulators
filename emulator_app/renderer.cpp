#include "renderer.hpp"

#include "shader_sources.inl"

#include <glad/gl.h>

namespace Renderer {

    struct CharVertex {
        u16 x, y;
        u32 color;
    };

    static glw::Shader* pointShader = nullptr;
    static CharVertex charVertices[64 * 32];
    static glw::VertexBuffer* charVBO = nullptr;
    static glw::VertexArray* charVAO = nullptr;

    static glw::Shader* textureShader = nullptr;
    static struct TextureVertex {
        f32 x, y;
        f32 u, v;
    } textureVertices[] = {
        -1.f, -1.f, 0.f, 0.f,
         1.f, -1.0f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 1.f
    };
    static u8 textureIndices[] = {
        0, 1, 2,
        2, 3, 0
    };
    static glw::VertexBuffer* textureVBO = nullptr;
    static glw::IndexBuffer* textureIBO = nullptr;
    static glw::VertexArray* textureVAO = nullptr;

    static glw::Framebuffer* FBO = nullptr;

	void init(u16 frameWidth, u16 frameHeight, glw::GLWLoadFunc loadFunc)
	{
		glw::init(loadFunc);

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
            glw::Framebuffer::Properties{ frameWidth, frameHeight, 1, {
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

}
