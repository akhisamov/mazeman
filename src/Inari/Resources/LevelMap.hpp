#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <string>
#include <vector>

#include "Inari/Resources/IResource.hpp"

namespace inari {
struct Tileset {
    int32_t columns;
    int32_t firstgid;
    std::string image;
    int32_t margin;
    int32_t spacing;
    uint32_t count;
    glm::vec2 size;
};

class LevelMap final : public IResource {
   protected:
    struct Token;

   public:
    static std::shared_ptr<LevelMap> createFromData(
        const std::string_view& data);

    explicit LevelMap(const std::unique_ptr<Token>& token);
    LevelMap() = delete;
    ~LevelMap() override;

    const glm::vec3& getBackgroundColor() const;
    const std::vector<Tileset>& getTilesets() const;

   private:
    glm::vec3 m_backgroundColor;
    std::vector<Tileset> m_tilesets;
};
}  // namespace inari