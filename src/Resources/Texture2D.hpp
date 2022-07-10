#pragma once

#include <glm/vec2.hpp>

#include <SDL_surface.h>

#include <memory>
#include <map>

#include "Resource.hpp"

class Texture2D :
        public Resource
{
public:
    static std::shared_ptr<Texture2D> create(SDL_Surface* surface);
    static std::shared_ptr<Texture2D> createFromData(const std::map<std::string, std::string>& data);
    static std::vector<std::string> getFiles(const std::string_view& name);

    Texture2D() = delete;
    ~Texture2D() override;

    void bind() const;

    [[nodiscard]] uint32_t getId() const
    {
        return m_id;
    }
    [[nodiscard]] const glm::vec2& getSize() const
    {
        return m_size;
    }

private:
    explicit Texture2D(uint32_t id, const glm::vec2& size);

    uint32_t m_id;
    glm::vec2 m_size;
};