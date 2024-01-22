#pragma once
#include <cstdint>
#include <String>

class Shader
{
public:
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    Shader(const Shader&) = delete;
    ~Shader();

    void bind() const;
    void unbind() const;

    //void set(const char* name, int value) const;
    //void set(const char* name, const int* value, uint32_t count) const;
    //void set(const char* name, const glm::vec3& value) const;
    //void set(const char* name, const glm::vec4& value) const;
    //void set(const char* name, const glm::mat4& value) const;
private:
    std::string readFile(const char* filepath);
    uint32_t compileShader(const char* shaderSource, uint32_t shaderType) const;

    uint32_t m_id;
};
