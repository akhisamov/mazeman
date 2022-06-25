#include "BundleReader.hpp"

#include <SDL_filesystem.h>

#include <nlohmann/json.hpp>

#include <zip.h>

#include "StringUtils.hpp"

using namespace nlohmann;

BundleReader::BundleReader(const std::string_view& bundleFile, ResourceType type)
{
    m_bundleFile = SDL_GetBasePath();
    m_bundleFile /= bundleFile;

    const std::string data = getFileContent(m_bundleFile.stem().string() + ".bundleconf");
    if (data.empty())
    {
        throw std::runtime_error("Bundle Config is not found");
    }

    json j = json::parse(data);
    for (const auto& it : j)
    {
        const std::string typeStr = it.at("type");
        ResourceType resType(typeStr.c_str());
        if (resType == ResourceType::NONE)
        {
            throw std::runtime_error("Bundle Config parsing error: Resource type is none");
        }
        if (resType == type)
        {
            BundleResource res;
            res.id = it.at("id");
            res.type = resType;
            for (const auto& p : it.at("data").items())
            {
                res.data.emplace(p.key(), p.value());
            }
            m_nameResources.emplace(it.at("name"), res);
        }
    }
}

std::string BundleReader::getFileContent(const std::string_view& filename)
{
    int err = 0;
    zip* z = zip_open(m_bundleFile.string().c_str(), ZIP_RDONLY, &err);
    if (z)
    {
        struct zip_stat st{};
        zip_stat_init(&st);
        zip_stat(z, filename.data(), 0, &st);

        std::string result;
        result.resize(static_cast<size_t>(st.size));

        zip_file* f = zip_fopen(z, filename.data(), 0);
        zip_fread(f, result.data(), st.size);
        zip_fclose(f);

        zip_close(z);
        return result;
    }

    return {};
}

const BundleResource& BundleReader::getResourceConfig(const std::string_view& name)
{
    const auto& it = m_nameResources.find(name.data());
    if (it != m_nameResources.end())
    {
        return it->second;
    }
    else
    {
        const std::string_view message = "Resource Load Error [%s]: resource is not found";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }
}
