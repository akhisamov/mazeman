#include "LevelMap.hpp"
#include <memory>
#include "Inari/Resources/LevelMap.hpp"

#define CUTE_TILED_IMPLEMENTATION
#include <cute_tiled.h>

#include "Inari/Utils/Colors.hpp"

namespace {

inari::LevelTileset createTileset(const cute_tiled_tileset_t& cuteTileset) {
    inari::LevelTileset tileset;
    tileset.columns = cuteTileset.columns;
    tileset.firstgid = cuteTileset.firstgid;
    tileset.image = cuteTileset.image.ptr;
    tileset.margin = cuteTileset.margin;
    tileset.spacing = cuteTileset.spacing;
    tileset.count = cuteTileset.tilecount;
    tileset.size = {cuteTileset.tilewidth, cuteTileset.tileheight};
    return tileset;
}

inari::LevelLayer createLayer(const cute_tiled_layer_t& cuteLayer) {
    inari::LevelLayer layer;
    layer.tiles =
        std::vector(cuteLayer.data, cuteLayer.data + cuteLayer.data_count);
    layer.visible = cuteLayer.visible;
    layer.position = {cuteLayer.x, cuteLayer.y};
    layer.size = {cuteLayer.width, cuteLayer.height};
    return layer;
}

}  // namespace

namespace inari {

struct LevelMap::Token {
    ~Token() {
        if (map) {
            cute_tiled_free_map(map);
        }
    }

    cute_tiled_map_t* map = nullptr;
};

std::shared_ptr<LevelMap> LevelMap::createFromData(
    const std::string_view& data) {
    auto token = std::make_unique<LevelMap::Token>();
    token->map = cute_tiled_load_map_from_memory(
        data.data(), static_cast<int>(data.size()), nullptr);
    if (token->map == nullptr) {
        return nullptr;
    }
    return std::make_shared<LevelMap>(token);
}

LevelMap::LevelMap(const std::unique_ptr<Token>& token)
    : m_backgroundColor(colors::toGL<3>(token->map->backgroundcolor)) {
    cute_tiled_tileset_t* tileset = token->map->tilesets;
    while (tileset) {
        m_tilesets.push_back(createTileset(*tileset));
        tileset = tileset->next;
    }

    cute_tiled_layer_t* layer = token->map->layers;
    while (layer) {
        m_layers.push_back(createLayer(*layer));
        layer = layer->next;
    }
}
LevelMap::~LevelMap() = default;

const glm::vec3& LevelMap::getBackgroundColor() const {
    return m_backgroundColor;
}

const std::vector<LevelLayer>& LevelMap::getLayers() const {
    return m_layers;
}

const std::vector<LevelTileset>& LevelMap::getTilesets() const {
    return m_tilesets;
}

std::unique_ptr<LevelTileset> LevelMap::getTilesetByGID(int32_t gid) const {
    for (const auto& tileset : m_tilesets) {
        if (tileset.firstgid <= gid &&
            gid < tileset.firstgid + static_cast<int>(tileset.count)) {
            return std::make_unique<LevelTileset>(tileset);
        }
    }
    return nullptr;
}

}  // namespace inari