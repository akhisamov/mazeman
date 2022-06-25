#pragma once

#include <glm/mat4x4.hpp>

#include <memory>
#include <string>

#include "Resource.hpp"

class Shader :
        public Resource
{
public:
    static std::shared_ptr<Shader>
    create(const std::string_view& resourceId, const std::string_view& vertexCode,
            const std::string_view& fragmentCode);

    Shader() = delete;
    ~Shader() override;

    void use() const;

    void set(const std::string_view& name, int value) const;
    void set(const std::string_view& name, bool value) const;
    void set(const std::string_view& name, float value) const;
    void set(const std::string_view& name, const glm::vec3& value) const;
    void set(const std::string_view& name, const glm::mat4& matrix) const;

private:
    explicit Shader(const std::string_view& resourceId, uint32_t id);

    uint32_t m_id;
};