#pragma once

#include <memory>

#include "BundleReader.hpp"

template<class T>
class BaseResourceManager :
        public BundleReader
{
public:
    explicit BaseResourceManager(const std::string& bundleFile);
protected:
    const std::shared_ptr<T>& baseLoad(const std::string& name);
    bool baseUnload(const std::string& name)
    {
        return m_resources.erase(name) == 1;
    }

    std::map<std::string, std::shared_ptr<T>> m_resources;
};