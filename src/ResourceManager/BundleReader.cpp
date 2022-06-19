#include "BundleReader.hpp"

#include <SDL_filesystem.h>

#include <boost/json/src.hpp>

#include <zip.h>

BundleReader::BundleReader(const std::string& bundleFile, ResourceType type)
{
    m_bundleFile = SDL_GetBasePath();
    m_bundleFile /= bundleFile;

    const std::string data = getFileContent(m_bundleFile.stem().string() + ".bundleconf");
    if (data.empty())
    {
        throw std::runtime_error("Bundle Config is not found");
    }

    boost::json::error_code ec;
    boost::json::value jv = boost::json::parse(data, ec);
    if (ec)
    {
        throw std::runtime_error("Bundle Config parsing error: " + ec.message());
    }

    for (const auto& it : jv.get_array())
    {
        const auto& resObject = it.get_object();
        ResourceType resType(resObject.at("type").get_string().c_str());
        if (resType == ResourceType::NONE)
        {
            throw std::runtime_error("Bundle Config parsing error: Resource type is none");
        }
        if (resType == type)
        {
            BundleResource res;
            res.id = resObject.at("id").get_string();
            res.type = resType;
            const auto& dataValue = resObject.at("data");
            for (auto p : dataValue.get_object())
            {
                res.data.emplace(p.key(), p.value().get_string());
            }
            m_nameResources.emplace(resObject.at("name").get_string(), res);
        }
    }
}

std::string BundleReader::getFileContent(const std::string& filename)
{
    int err = 0;
    zip* z = zip_open(m_bundleFile.string().c_str(), ZIP_RDONLY, &err);
    if (z)
    {
        struct zip_stat st{};
        zip_stat_init(&st);
        zip_stat(z, filename.c_str(), 0, &st);

        std::string result;
        result.resize(static_cast<size_t>(st.size));

        zip_file* f = zip_fopen(z, filename.c_str(), 0);
        zip_fread(f, result.data(), st.size);
        zip_fclose(f);

        zip_close(z);
        return result;
    }

    return {};
}

const BundleResource& BundleReader::getResourceConfig(const std::string& name)
{
    const auto& it = m_nameResources.find(name);
    if (it != m_nameResources.end())
    {
        return it->second;
    }
    else
    {
        const std::string message = "Resource Load Error [" + name + "]: resource is not found";
        throw std::runtime_error(message);
    }
}
