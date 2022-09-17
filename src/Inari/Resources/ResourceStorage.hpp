#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace inari {
template <class T>
class ResourceStorage {
   public:
    virtual ~ResourceStorage() { m_resources.clear(); }

   protected:
    using ResourcesMap = std::map<std::string_view, std::shared_ptr<T>>;
    using FilesMap = std::map<std::string_view, std::vector<std::string_view>>;

    std::vector<std::string_view> getFiles(const std::string_view& name) {
        auto& files = ResourceStorage<T>::m_files;
        if (!files.empty()) {
            const auto& it = files.find(name);
            if (it != files.end()) {
                return it->second;
            }
        }
        return {};
    }

    ResourcesMap m_resources;
    FilesMap m_files;
};
}  // namespace inari