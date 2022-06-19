#pragma once

#include "Resources/ResourceType.hpp"

#include <map>
#include <string>
#include <filesystem>

using BundleData = std::map<std::string, std::string>;

struct BundleResource
{
    std::string id;
    ResourceType type;
    std::map<std::string, std::string> data;
};

class BundleReader
{
public:
    explicit BundleReader(const std::string& bundleFile, ResourceType type);
    virtual ~BundleReader() = default;

protected:
    std::string getFileContent(const std::string& filename);
    const BundleResource& getResourceConfig(const std::string& name);

    std::filesystem::path m_bundleFile;

private:
    std::map<std::string, BundleResource> m_nameResources;
};
