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
        else
        {
            glGetProgramiv(id, type, &success);
        }
        if (!success)
        {
            char infoLog[512];
            if (type == GL_COMPILE_STATUS)
            {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
            }
            else
            {
                glGetProgramInfoLog(id, 512, nullptr, infoLog);
            }
            std::cerr << "Shader error: " << infoLog << std::endl;
            return false;
        }
        return true;
    }

    constexpr std::string_view vertexExt = ".vert";
    constexpr std::string_view fragmentExt = ".frag";
}

std::shared_ptr<Shader> Shader::create(const std::string_view& vertexCode, const std::string_view& fragmentCode)
{
    Shader::Data data;
    uint32_t vertexShader = 0;
    uint32_t fragmentShader = 0;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexCodeStr = vertexCode.data();
    glShaderSource(vertexShader, 1, &vertexCodeStr, nullptr);
    glCompileShader(vertexShader);
    if (checkShaderStatus(vertexShader, GL_COMPILE_STATUS))
    {

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentCodeStr = fragmentCode.data();
        glShaderSource(fragmentShader, 1, &fragmentCodeStr, nullptr);
        glCompileShader(fragmentShader);

        if (checkShaderStatus(fragmentShader, GL_COMPILE_STATUS))
        {

            data.id = glCreateProgram();
            glAttachShader(data.id, vertexShader);
            glAttachShader(data.id, fragmentShader);
            glLinkProgram(data.id);

            if (!checkShaderStatus(data.id, GL_LINK_STATUS))
            {
                data.id = 0;
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
    if (data.id != 0)
    {
        return std::make_shared<Shader>(data);
    }

    return nullptr;
}

std::shared_ptr<Shader> Shader::createFromData(const std::map<std::string, std::string>& data)
{
    std::string vertexCode;
    std::string fragmentCode;

    for (const auto& it : data)
    {
        const std::string_view& filename = it.first;
        if (it.first.rfind(vertexExt) == it.first.size() - vertexExt.size())
        {
            vertexCode = it.second;
        }
        else if (it.first.rfind(fragmentExt) == it.first.size() - fragmentExt.size())
        {
            fragmentCode = it.second;
        }
    }

    if (vertexCode.empty() || fragmentCode.empty())
    {
        throw std::runtime_error("Creation Shader from data ERROR: resource is not found");
    }

    return create(vertexCode, fragmentCode);
}

Shader::Shader(const Data& data)
    : m_id(data.id)
{
}

Shader::~Shader() { glDeleteProgram(m_id); }

void Shader::use() const { glUseProgram(m_id); }

void Shader::set(const std::string_view& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.data()), value);
}

void Shader::set(const std::string_view& name, bool value) const { set(name, static_cast<int>(value)); }

void Shader::set(const std::string_view& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.data()), value);
}

void Shader::set(const std::string_view& name, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(m_id, name.data()), value.x, value.y, value.z);
}

void Shader::set(const std::string_view& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, false, glm::value_ptr(matrix));
}
