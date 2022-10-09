#include "LevelMap.hpp"

#include "Inari/Resources/LevelMap.hpp"

#include <nlohmann/json.hpp>

#include "Inari/Utils/Colors.hpp"

namespace {

// inari::LevelTileset createTileset(const cute_tiled_tileset_t& cuteTileset) {
//     inari::LevelTileset tileset;
//     tileset.columns = cuteTileset.columns;
//     tileset.firstgid = cuteTileset.firstgid;
//     tileset.image = cuteTileset.image.ptr;
//     tileset.margin = cuteTileset.margin;
//     tileset.spacing = cuteTileset.spacing;
//     tileset.count = cuteTileset.tilecount;
//     tileset.size = {cuteTileset.tilewidth, cuteTileset.tileheight};
//     return tileset;
// }

// enum class LayerType { NONE = 0, TILE_LAYER, OBJECT_GROUP, IMAGE_LAYER, GROUP
// }; LayerType getLayerType(const cute_tiled_string_t& typeStr) {
//     constexpr std::string_view tilelayer = "tilelayer";
//     constexpr std::string_view objectgroup = "objectgroup";
//     constexpr std::string_view imagelayer = "imagelayer";
//     constexpr std::string_view group = "group";

//     if (typeStr.ptr == tilelayer) {
//         return LayerType::TILE_LAYER;
//     }
//     if (typeStr.ptr == objectgroup) {
//         return LayerType::OBJECT_GROUP;
//     }
//     if (typeStr.ptr == imagelayer) {
//         return LayerType::IMAGE_LAYER;
//     }
//     if (typeStr.ptr == group) {
//         return LayerType::GROUP;
//     }

//     return LayerType::NONE;
// }

// inari::LevelLayerPtr createLayer(const cute_tiled_layer_t* cuteLayer) {
//     if (cuteLayer == nullptr) {
//         return nullptr;
//     }

//     inari::LevelLayerPtr layer = nullptr;

//     switch (getLayerType(cuteLayer->type)) {
//         case LayerType::NONE:
//             return nullptr;
//         case LayerType::TILE_LAYER: {
//             auto tileLayer = std::make_shared<inari::LevelTileLayer>();
//             tileLayer->tiles = std::vector(
//                 cuteLayer->data, cuteLayer->data + cuteLayer->data_count);
//             layer = std::move(tileLayer);
//             break;
//         }
//         case LayerType::OBJECT_GROUP:
//         case LayerType::IMAGE_LAYER:
//         case LayerType::GROUP:
//             return nullptr;
//     }

//     layer->visible = cuteLayer->visible;
//     layer->position = {cuteLayer->x, cuteLayer->y};
//     layer->size = {cuteLayer->width, cuteLayer->height};

//     return layer;
// }

}  // namespace

namespace inari {

struct LevelMap::Token {
    glm::vec3 backgroundColor;
};

std::shared_ptr<LevelMap> LevelMap::createFromData(
    const std::string_view& data) {
    auto jsonData = nlohmann::json::parse(data);

    for (auto& [key, value] : jsonData.items()) {
        if (key == "backgroundcolor") {
            // toGL(value);
        }
    }

    return std::make_shared<LevelMap>(LevelMap::Token{});
}

LevelMap::LevelMap(Token)
    : m_backgroundColor(/* colors::toGL<3>(token->map->backgroundcolor) */) {
    // cute_tiled_tileset_t* tileset = token->map->tilesets;
    // while (tileset) {
    //     m_tilesets.push_back(createTileset(*tileset));
    //     tileset = tileset->next;
    // }

    // cute_tiled_layer_t* layer = token->map->layers;
    // while (layer) {
    //     const LevelLayerPtr levelLayer = createLayer(layer);
    //     if (levelLayer) {
    //         m_layers.push_back(levelLayer);
    //     }
    //     layer = layer->next;
    // }
}
LevelMap::~LevelMap() = default;

const glm::vec3& LevelMap::getBackgroundColor() const {
    return m_backgroundColor;
}

const std::vector<LevelLayerPtr>& LevelMap::getLayers() const {
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