#pragma once

#include <glm/mat4x4.hpp>

#include <map>
#include <memory>
#include <string>

#include "Inari/Resources/IResource.hpp"

namespace inari {
class Shader final : public IResource {
   protected:
    struct Data {
        uint32_t id = 0;
    };

   public:
    static std::shared_ptr<Shader> create(const std::string_view& vertexCode,
                                          const std::string_view& fragmentCode);
    static std::shared_ptr<Shader> createFromData(const std::string_view& data);

    explicit Shader(const Data& data);
    Shader() = delete;
    ~Shader() override;

    void use() const;

    void set(const std::string_view& name, int value) const;
    void set(const std::string_view& name, bool value) const;
    void set(const std::string_view& name, float value) const;
    void set(const std::string_view& name, const glm::vec2& value) const;
    void set(const std::string_view& name, const glm::vec3& value) const;
    void set(const std::string_view& name, const glm::vec4& value) const;
    void set(const std::string_view& name, const glm::mat4& matrix) const;

   private:
    uint32_t m_id;
};
}  // namespace inari
