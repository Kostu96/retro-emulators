#include "renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Renderer::Renderer(GLFWwindow* window) :
    m_window(window),
    m_gouraudVBO(6 * sizeof(GouraudVertex)),
    m_textureVBO(6 * sizeof(TextureVertex)),
	m_gouraudShader("assets/gouraud_vertex.glsl", "assets/gouraud_fragment.glsl"),
	m_textureShader("assets/texture_vertex.glsl", "assets/texture_fragment.glsl")
{

	// Set vertex layout for Gouraud shader
    m_gouraudVBO.bind();
	glGenVertexArrays(1, &m_gouraudVAO);
	glBindVertexArray(m_gouraudVAO);

	glEnableVertexAttribArray(0);
    glVertexAttribIPointer(
        0,
        2,
        GL_SHORT,
        sizeof(GouraudVertex),
        0
    ); // position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(GouraudVertex),
        (const void*)(sizeof(int16_t) * 2)
    ); // color

    // Set vertex layout for texture shader
    m_textureVBO.bind();
    glGenVertexArrays(1, &m_textureVAO);
    glBindVertexArray(m_textureVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(
        0,
        2,
        GL_SHORT,
        sizeof(TextureVertex),
        0
    ); // position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TextureVertex),
        (const void*)(sizeof(int16_t) * 2)
    ); // texCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(TextureVertex),
        (const void*)((sizeof(int16_t) + sizeof(float)) * 2)
    ); // color

    glClearColor(1.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    swap();
}

void Renderer::swap() const
{
    glfwSwapBuffers(m_window);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::pushImage(const ImageData& imageData) const
{
    /*uint32_t m_texture;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.image.data());

    Renderer::TextureVertex vertices[6];
    vertices[0].x = imageData.destinationX;
    vertices[0].y = imageData.destinationY;
    vertices[0].texCoordX = 0.f;
    vertices[0].texCoordY = 0.f;

    vertices[1].x = imageData.destinationX + imageData.width;
    vertices[1].y = imageData.destinationY;
    vertices[1].texCoordX = 1.f;
    vertices[1].texCoordY = 0.f;

    vertices[2].x = imageData.destinationX;
    vertices[2].y = imageData.destinationY + imageData.height;
    vertices[2].texCoordX = 0.f;
    vertices[2].texCoordY = 1.f;

    vertices[3].x = imageData.destinationX;
    vertices[3].y = imageData.destinationY;

    vertices[4].x = imageData.destinationX + imageData.width;
    vertices[4].y = imageData.destinationY;

    vertices[5].x = imageData.destinationX;
    vertices[5].y = imageData.destinationY + imageData.height;
    vertices[5].texCoordX = 0.f;
    vertices[5].texCoordY = 0.f;

    m_textureShader.bind();
    glBindVertexArray(m_textureVAO);
    m_textureVBO.setData(vertices, sizeof(vertices));
    glDrawArrays(GL_TRIANGLES, 0, 6);*/
}

void Renderer::pushTriangle(const void* vertices, size_t size) const
{
    m_gouraudShader.bind();
    glBindVertexArray(m_gouraudVAO);
    m_gouraudVBO.setData(vertices, size);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::pushQuad(const void* vertices, size_t size) const
{
    m_gouraudShader.bind();
    glBindVertexArray(m_gouraudVAO);
    m_gouraudVBO.setData(vertices, size);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
