#include "shader.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string code = readFile(vertexShaderPath);
	uint32_t vs = compileShader(code.data(), GL_VERTEX_SHADER);

	code = readFile(fragmentShaderPath);
	uint32_t fs = compileShader(code.data(), GL_FRAGMENT_SHADER);

	m_id = glCreateProgram();
	glAttachShader(m_id, vs);
	glAttachShader(m_id, fs);

	glLinkProgram(m_id);
	int success;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		int maxLength = 0;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(m_id, maxLength, nullptr, infoLog);
		std::cerr << "Shader program linking failed! " << infoLog;
		delete[] infoLog;
		abort();
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::bind() const
{
	glUseProgram(m_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

//void Shader::set(const char* name, int value) const
//{
//	glUniform1i(glGetUniformLocation(m_id, name), value);
//}
//
//void Shader::set(const char* name, const int* value, uint32 count) const
//{
//	glUniform1iv(glGetUniformLocation(m_id, name), static_cast<int>(count), value);
//}
//
//void Shader::set(const char* name, const glm::vec3& value) const
//{
//	glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
//}
//
//void Shader::set(const char* name, const glm::vec4& value) const
//{
//	glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
//}
//
//void Shader::set(const char* name, const glm::mat4& value) const
//{
//	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
//}

// TODO: merge this with BIOS implementation and put in some helpers
std::string Shader::readFile(const char* filepath)
{
	std::string result;
	std::ifstream fin(filepath);
	if (!fin.is_open()) {
		std::cerr << "Failed to open file: " << filepath << '\n';
		abort();
	}

	fin.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize size = fin.gcount();
	result.resize(size);
	fin.clear();
	fin.seekg(0);
	if (!fin.read(&result[0], size)) {
		std::cerr << "Failed to read from: " << filepath << "\nFile size was: " << size << '\n';
		abort();
	}

	fin.close();
	return result;
}

uint32_t Shader::compileShader(const char* shaderSource, uint32_t shaderType) const
{
	int success;
	uint32_t id = glCreateShader(shaderType);

	glShaderSource(id, 1, &shaderSource, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		int maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(id, maxLength, nullptr, infoLog);
		std::cerr << "Shader compilation failed!\n" << infoLog;
		delete[] infoLog;
		abort();
	};

	return id;
}
