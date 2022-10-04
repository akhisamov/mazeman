#include "LevelMap.hpp"

#define CUTE_TILED_IMPLEMENTATION
#include <cute_tiled.h>

#include <vector>

namespace inari {

struct LevelMap::Data {
    ~Data() { cute_tiled_free_map(map); }

    cute_tiled_map_t* map = nullptr;
};

std::shared_ptr<LevelMap> LevelMap::createFromData(
    const std::string_view& data) {
    auto levelData = std::make_unique<LevelMap::Data>();
    levelData->map = cute_tiled_load_map_from_memory(
        data.data(), static_cast<int>(data.size()), nullptr);
    if (levelData->map == nullptr) {
        return nullptr;
    }
    return std::make_shared<LevelMap>(std::move(levelData));
}

LevelMap::LevelMap(std::unique_ptr<Data>&& data) : m_data(std::move(data)) {}
LevelMap::~LevelMap() = default;

int LevelMap::getBackgroundColor() const {
    return m_data->map->backgroundcolor;
}

void LevelMap::forEachLayer() {
    cute_tiled_layer_t* layer = m_data->map->layers;
    while (layer) {
        if (layer->visible) {
            const std::vector<int> data(layer->data,
                                        layer->data + layer->data_count);
        }

        layer = layer->next;
    }
}

}  // namespace inari