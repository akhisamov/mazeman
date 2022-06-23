#pragma once

#include "Resources/ResourceType.hpp"

#include <map>
#include <string>
#include <filesystem>

struct BundleResource
{
    std::string id;
    ResourceType type;
    std::map<std::string, std::string> data;
};

class BundleReader
{
public:
    explicit BundleReader(const std::string_view& bundleFile, ResourceType type);
    virtual ~BundleReader() = default;

protected:
    std::string getFileContent(const std::string_view& filename);
    const BundleResource& getResourceConfig(const std::string_view& name);

    std::filesystem::path m_bundleFile;

private:
    std::map<std::string, BundleResource> m_nameResources;
};
