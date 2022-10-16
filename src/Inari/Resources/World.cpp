#include "World.hpp"

#include <nlohmann/json.hpp>

#include "Inari/Utils/Colors.hpp"

namespace {
constexpr std::string_view levelPrefix = "Level_";
}

namespace inari {

void from_json(const nlohmann::json& j, LevelTile& tile) {
    j.at("px").at(0).get_to(tile.position.x);
    j.at("px").at(1).get_to(tile.position.y);
    j.at("src").at(0).get_to(tile.sourceRect.x);
    j.at("src").at(1).get_to(tile.sourceRect.y);
}

void from_json(const nlohmann::json& j, LevelLayer& layer) {
    j.at("__identifier").get_to(layer.identifier);

    int gridSize = 0;
    j.at("__gridSize").get_to(gridSize);

    for (auto& element : j.at("gridTiles")) {
        LevelTile tile{};
        element.get_to(tile);
        tile.sourceRect.z = static_cast<float>(gridSize);
        tile.sourceRect.w = static_cast<float>(gridSize);
        layer.gridTiles.push_back(tile);
    }
}

void from_json(const nlohmann::json& j, WorldLevel& level) {
    j.at("identifier").get_to(level.identifier);
    std::string bgColor;
    j.at("bgColor").get_to(bgColor);
    level.backgroundColor = colors::toGL<3>(bgColor);
    j.at("pxWid").get_to(level.size.x);
    j.at("pxHei").get_to(level.size.y);

    for (auto& element : j.at("layerInstances")) {
        LevelLayer layer;
        element.get_to(layer);
        level.layers[layer.identifier] = layer;
    }
}

struct World::Data {
    std::map<std::string, WorldLevel> levels;

    friend void from_json(const nlohmann::json& j, World::Data& data) {
        for (auto& element : j.at("levels")) {
            WorldLevel level;
            element.get_to(level);
            data.levels[level.identifier] = level;
        }
    }
};

std::shared_ptr<World> World::createFromData(const std::string_view& data) {
    auto jsonData = nlohmann::json::parse(data);
    auto worldData = std::make_unique<Data>(jsonData.get<World::Data>());
    return std::make_shared<World>(std::move(worldData));
}

World::World(std::unique_ptr<Data>&& data) : m_data(std::move(data)) {}
World::~World() = default;

const WorldLevel& World::getLevel(int idx) const {
    const auto it =
        m_data->levels.find(levelPrefix.data() + std::to_string(idx));
    assert(it != m_data->levels.end());
    return it->second;
}

}  // namespace inari