#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <any>
#include <map>
#include <memory>
#include <vector>

#include "Inari/Resources/IResource.hpp"

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

    class World : public IResource {
    protected:
        struct Data;

    public:
        static std::shared_ptr<World> createFromData(const std::string_view& data);

        explicit World(std::unique_ptr<Data>&& data);
        World() = delete;
        ~World() override;

        const WorldLevel& getLevel(int idx) const;

    private:
        std::unique_ptr<Data> m_data;
    };

} // namespace inari