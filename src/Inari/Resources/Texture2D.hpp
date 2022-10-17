#pragma once

#include <glm/vec2.hpp>

#include <SDL_surface.h>

#include <map>
#include <memory>
#include <string>

#include "IResource.hpp"

namespace inari {
class Texture2D final : public IResource {
   protected:
    struct Data {
        uint32_t id = 0;
        glm::vec2 size;
    };

   public:
    static std::shared_ptr<Texture2D> create(SDL_Surface* surface);
    static std::shared_ptr<Texture2D> createFromData(
        const std::string_view& data);

    explicit Texture2D(const Data& data);
    Texture2D() = delete;
    ~Texture2D() override;

    void bind() const;

    [[nodiscard]] uint32_t getId() const { return m_id; }
    [[nodiscard]] const glm::vec2& getSize() const { return m_size; }

    static uint32_t getId(const std::shared_ptr<Texture2D>& texture);

   private:
    uint32_t m_id;
    glm::vec2 m_size;
};
}  // namespace inari
