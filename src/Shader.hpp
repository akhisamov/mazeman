#pragma once

#include <glm/mat4x4.hpp>

#include <memory>
#include <string>

#include "Resource.hpp"

class Shader :
    public Resource
{
  public:
    static std::shared_ptr<Shader> create(uint64_t resourceId, const std::string &vertexCode, const std::string &fragmentCode);

    Shader() = delete;
    ~Shader() override;

    void use() const;

    void set(const std::string& name, int value) const;
    void set(const std::string& name, bool value) const;
    void set(const std::string& name, float value) const;
    void set(const std::string& name, const glm::vec3& value) const;
    void set(const std::string& name, const glm::mat4& matrix) const;

  private:
    explicit Shader(uint64_t resourceId, uint32_t id);

    uint32_t m_id;
};