#pragma once

#include <map>
#include <string>
#include <filesystem>

class BundleReader
{
public:
    virtual ~BundleReader() = default;

protected:
    std::map<std::string, std::string>
    readBundle(const std::string& viewName, const std::string& resourceName, int& resourceId);

    void setBundleFile(const std::filesystem::path& path)
    {
        m_bundleFile = path;
    }

    std::filesystem::path m_bundleFile;
};
