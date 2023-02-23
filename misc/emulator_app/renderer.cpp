#include "renderer.hpp"

#include "shader_sources.inl"

#include <glad/gl.h>

namespace Renderer {

    struct PointVertex {
        u16 x, y;
        u32 color;
    };
    static_assert(sizeof(PointVertex) == 8);

    static struct RendererData
    {
        u16 maxPointVerticesPerBatch = 0;
        PointVertex* pointVBOBase = nullptr;
        PointVertex* pointVBOPtr = nullptr;
        glw::Shader* pointShader = nullptr;
        glw::VertexArray* pointVAO = nullptr;
        glw::VertexBuffer* pointVBO = nullptr;
    }
    s_data;

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

    static void beginPointBatch()
    {
        s_data.pointVBOPtr = s_data.pointVBOBase;
    }

    static void endPointBatch()
    {
        size_t dataSize = (uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase;
        if (dataSize > 0)
        {
            s_data.pointVBO->setData(s_data.pointVBOBase, dataSize);
            s_data.pointVAO->bind();
            s_data.pointShader->bind();
            glDrawArrays(GL_POINTS, 0, (GLsizei)(dataSize / sizeof(PointVertex)));
        }
    }

    void init(u16 frameWidth, u16 frameHeight, glw::GLWLoadFunc loadFunc)
    {
        glw::init(loadFunc);

        glActiveTexture(GL_TEXTURE0);

        s_data.pointShader = new glw::Shader{};
        s_data.pointShader->createFromSource(pointVertSource, pointFragSource);

        s_data.maxPointVerticesPerBatch = frameWidth * frameHeight;
        s_data.pointVBOBase = new PointVertex[s_data.maxPointVerticesPerBatch];
        s_data.pointVBO = new glw::VertexBuffer{ s_data.maxPointVerticesPerBatch * sizeof(PointVertex) };
        glw::VertexLayout pointLayout{ {
            { glw::LayoutElement::DataType::U16_2, false },
            { glw::LayoutElement::DataType::U8_4, true, true }
        } };
        s_data.pointVAO = new glw::VertexArray{ *s_data.pointVBO, pointLayout };

        textureVBO = new glw::VertexBuffer{ textureVertices, sizeof(textureVertices) };
        textureIBO = new glw::IndexBuffer{ textureIndices, sizeof(textureIndices) / sizeof(u8), glw::IndexBuffer::IndexType::U8 };
        glw::VertexLayout texturelayout{ {
            { glw::LayoutElement::DataType::F32_2 },
            { glw::LayoutElement::DataType::F32_2 }
        } };
        textureVAO = new glw::VertexArray{ *textureVBO, texturelayout };
        textureVAO->setIndexBuffer(*textureIBO);

        textureShader = new glw::Shader{};
        textureShader->createFromSource(textureVertSource, textureFragSource);

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
        delete textureShader;

        delete s_data.pointVAO;
        delete s_data.pointVBO;
        delete s_data.pointShader;
        delete[] s_data.pointVBOBase;
    }

    void beginFrame()
    {
        FBO->bind();
        glViewport(0, 0, FBO->getProperties().width, FBO->getProperties().height);

        beginPointBatch();
    }

    void renderFrame(s32 x, s32 y, s32 width, s32 height)
    {
        endPointBatch();

        FBO->unbind();
        textureVAO->bind();
        textureShader->bind();
        glViewport(x, y, width, height);
        FBO->getAttachments()[0].bind(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
    }

    void clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void renderPoint(u16 x, u16 y, u32 color)
    {
        if (((uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase) / sizeof(PointVertex)
            >= s_data.maxPointVerticesPerBatch)
        {
            endPointBatch();
            beginPointBatch();
        }

        s_data.pointVBOPtr->x = x;
        s_data.pointVBOPtr->y = y;
        s_data.pointVBOPtr->color = color;
        s_data.pointVBOPtr++;
    }

}
