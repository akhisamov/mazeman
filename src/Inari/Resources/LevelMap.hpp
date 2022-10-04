#pragma once

#include <memory>
#include <string>

#include "Inari/Resources/IResource.hpp"

namespace inari {
class LevelMap final : public IResource {
   protected:
    struct Data;

   public:
    static std::shared_ptr<LevelMap> createFromData(
        const std::string_view& data);

    explicit LevelMap(std::unique_ptr<Data>&& data);
    LevelMap() = delete;
    ~LevelMap() override;

    int getBackgroundColor() const;
    void forEachLayer();

   private:
    std::unique_ptr<Data> m_data;
};
}  // namespace inari