#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace inari {
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

    void Shader::set(const std::string_view& name, const glm::vec2& value) const
    {
        glUniform2f(glGetUniformLocation(m_id, name.data()), value.x, value.y);
    }

    void Shader::set(const std::string_view& name, const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(m_id, name.data()), value.x, value.y, value.z);
    }

    void Shader::set(const std::string_view& name, const glm::vec4& value) const
    {
        glUniform4f(glGetUniformLocation(m_id, name.data()), value.r, value.g, value.b, value.a);
    }

    void Shader::set(const std::string_view& name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, false, glm::value_ptr(matrix));
    }
    int32_t Shader::getAttributePosition(const std::string_view& name) const
    {
        return glGetAttribLocation(m_id, name.data());
    }
} // namespace inari
