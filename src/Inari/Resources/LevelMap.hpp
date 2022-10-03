#pragma once

#include <memory>
#include <string>

namespace inari {
class LevelMap {
   protected:
    struct Data;

   public:
    static std::shared_ptr<LevelMap> create(const std::string_view& data);

    explicit LevelMap(std::unique_ptr<Data>&& data);
    LevelMap() = delete;
    ~LevelMap();

   private:
    std::unique_ptr<Data> m_data;
};
}  // namespace inari