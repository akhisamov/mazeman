#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Inari/Assets/IAsset.h"

namespace inari {
    struct LevelTile {
        glm::vec2 position;
        glm::vec4 sourceRect;
    };

    struct LevelEntityInstance {
        glm::vec2 position;
        std::map<std::string, std::any> fields;

        template <typename T>
        T get(const std::string_view& field) const
        {
            const auto it = fields.find(field.data());
            assert(it != fields.end());
            return std::any_cast<T>(it->second);
        }
    };

    struct LevelLayer {
        std::string identifier;
        std::vector<LevelTile> tiles;
        std::map<std::string, LevelEntityInstance> entityInstances;
    };

    struct WorldLevel {
        std::string identifier;
        glm::vec3 backgroundColor;
        glm::vec2 size;
        std::map<std::string, LevelLayer> layers;
    };

    class World final : public IAsset {
        friend class WorldMaker;

    public:
        const WorldLevel& getLevel(int idx) const;

    protected:
        struct Data {
            std::map<std::string, WorldLevel> levels;
        };

    public:
        explicit World(std::unique_ptr<Data>&& data);
        ~World() override;

        World() = delete;
        World(World&&) = delete;
        World(const World&) = delete;
        World& operator=(World&&) = delete;
        World& operator=(const World&) = delete;

    private:
        std::unique_ptr<Data> m_data;
    };
}