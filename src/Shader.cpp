#include "Shader.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace
{
bool checkShaderStatus(uint32_t id, GLenum type)
{
    int success = 0;
    if (type == GL_COMPILE_STATUS)
    {
        glGetShaderiv(id, type, &success);
    }
    else {
        glGetProgramiv(id, type, &success);
    }
    if (!success)
    {
        char infoLog[512];
        if (type == GL_COMPILE_STATUS)
        {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
        }
        else {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
        }
        std::cerr << "Shader error: " << infoLog << std::endl;
        return false;
    }
    return true;
}
}

std::shared_ptr<Shader> Shader::create(uint64_t resourceId, const std::string &vertexCode, const std::string &fragmentCode)
{
    uint32_t vertexShader = 0;
    uint32_t fragmentShader = 0;
    uint32_t shaderProgram = 0;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexCodeStr = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vertexCodeStr, nullptr);
    glCompileShader(vertexShader);
    if (checkShaderStatus(vertexShader, GL_COMPILE_STATUS))
    {

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentCodeStr = fragmentCode.c_str();
        glShaderSource(fragmentShader, 1, &fragmentCodeStr, nullptr);
        glCompileShader(fragmentShader);

        if (checkShaderStatus(fragmentShader, GL_COMPILE_STATUS))
        {

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            if (!checkShaderStatus(shaderProgram, GL_LINK_STATUS))
            {
                shaderProgram = 0;
            }
        }
    }

    if (vertexShader != 0)
    {
        glDeleteShader(vertexShader);
    }
    if (fragmentShader != 0)
    {
        glDeleteShader(fragmentShader);
    }
    if (shaderProgram != 0)
    {
        return std::shared_ptr<Shader>(new Shader(resourceId, shaderProgram));
    }

    return nullptr;
}

Shader::Shader(uint64_t resourceId, uint32_t id) :
    Resource(resourceId),
    m_id(id)
{
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::use() const
{
    glUseProgram(m_id);
}

void Shader::set(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set(const std::string &name, bool value) const
{
    set(name, static_cast<int>(value));
}

void Shader::set(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set(const std::string &name, const glm::vec3 &value)
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void Shader::set(const std::string &name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, false, glm::value_ptr(matrix));
}
