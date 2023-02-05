#pragma once

#include <glm/mat4x4.hpp>

#include <string_view>

#include "Inari/Assets/IAsset.h"

namespace inari {
    class Shader final : public IAsset {
        friend class ShaderMaker;

    public:
        void use() const;

        void set(const std::string_view& name, int value) const;
        void set(const std::string_view& name, bool value) const;
        void set(const std::string_view& name, float value) const;
        void set(const std::string_view& name, const glm::vec2& value) const;
        void set(const std::string_view& name, const glm::vec3& value) const;
        void set(const std::string_view& name, const glm::vec4& value) const;
        void set(const std::string_view& name, const glm::mat4& matrix) const;

        int32_t getAttributePosition(const std::string_view& name) const;

    protected:
        struct Data {
            uint32_t id = 0;
        };

    public:
        explicit Shader(const Data& data);
        ~Shader() override;

        Shader() = delete;
        Shader(Shader&&) = delete;
        Shader(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;

    private:
        uint32_t m_id;
    };
}
