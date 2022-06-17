#include "ResourceManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <sqlite3.h>

#include <iostream>

#include "Texture2D.hpp"
#include "Shader.hpp"

ResourceManager::ResourceManager(const std::string& bundleFile)
{
    m_bundleFile = SDL_GetBasePath();
    m_bundleFile /= bundleFile;
}

template<>
const std::shared_ptr<Texture2D>& ResourceManager::load(const std::string& name)
{
    const auto& it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return it->second;
    }

    int resourceId = 0;
    SDL_Surface* surface = nullptr;

    const auto data = readBundle("textures_v", name, resourceId);
    auto dataIt = data.find("file");
    if (dataIt != data.end())
    {
        SDL_RWops* rw = SDL_RWFromConstMem(dataIt->second.c_str(), dataIt->second.size());
        surface = IMG_Load_RW(rw, 1);
        if (surface == nullptr)
        {
            const std::string message = "Resource Load Error [" + name + "]: " + IMG_GetError();
            throw std::runtime_error(message);
        }
    }

    const auto& pair = m_textures.emplace(name, Texture2D::create(resourceId, surface));
    SDL_FreeSurface(surface);
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}

template<>
const std::shared_ptr<Shader>& ResourceManager::load(const std::string& name)
{
    const auto& it = m_shader.find(name);
    if (it != m_shader.end())
    {
        return it->second;
    }

    std::string vertexCode;
    std::string fragmentCode;
    int resourceId = 0;

    const auto data = readBundle("shaders_v", name, resourceId);
    if (!data.empty())
    {
        auto vertexIt = data.find("vertex");
        auto fragmentIt = data.find("fragment");
        if (vertexIt != data.end() && fragmentIt != data.end())
        {
            vertexCode = vertexIt->second;
            fragmentCode = fragmentIt->second;
        }
    }

    if (data.empty() || fragmentCode.empty() || vertexCode.empty())
    {
        const std::string message = "Resource Load Error [" + name + "]: resource is not found";
        throw std::runtime_error(message);
    }

    const auto& pair = m_shader.emplace(name, Shader::create(resourceId, vertexCode, fragmentCode));
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}

template<>
bool ResourceManager::unload<Texture2D>(const std::string &name)
{
    return m_textures.erase(name) == 1;
}

template<>
bool ResourceManager::unload<Shader>(const std::string& name)
{
    return m_shader.erase(name) == 1;
}

std::map<std::string, std::string> ResourceManager::readBundle(const std::string &viewName, const std::string &resourceName, int& resourceId)
{
    std::map<std::string, std::string> data;
    try
    {
        sqlite3* db;
        int rc = sqlite3_open_v2(m_bundleFile.string().c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
        if (rc == SQLITE_OK)
        {
            std::string query = "SELECT * FROM " + viewName + " WHERE Name = ?";
            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK)
            {
                rc = sqlite3_bind_text(stmt, 1, resourceName.c_str(), resourceName.size(), nullptr);
                if (rc == SQLITE_OK)
                {
                    rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW)
                    {
                        resourceId = sqlite3_column_int(stmt, 0);
                        for (int i = 2; i < sqlite3_column_count(stmt); ++i)
                        {
                            const char* key = sqlite3_column_name(stmt, i);
                            const void* value = sqlite3_column_blob(stmt, i);
                            int size = sqlite3_column_bytes(stmt, i);
                            data[key] = std::string(static_cast<const char*>(value), size);
                        }
                        rc = SQLITE_OK;
                    }
                    else if (rc == SQLITE_DONE)
                    {
                        sqlite3_finalize(stmt);
                        sqlite3_close(db);
                        throw std::runtime_error("Not found");
                    }
                }
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);
        }

        if (rc != SQLITE_OK)
        {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Resource Load Error [" << resourceName << "]: " << e.what() << std::endl;
    }
    return data;
}
