#pragma once

#include <string>
#include <vector>

class Resource
{
public:
    virtual ~Resource() = default;

    static std::vector<std::string> getFiles(const std::string_view& name, const std::vector<std::string_view>& exts)
    {
        std::vector<std::string> result;

        for (auto ext : exts)
        {
            std::string nameWithExt = std::string(name) + std::string(ext);
            result.push_back(nameWithExt);
        }

        return result;
    }
};