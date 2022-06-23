#pragma once

#include <memory>

#include "BundleReader.hpp"

template<class T>
class BaseResourceManager :
        public BundleReader
{
public:
    explicit BaseResourceManager(const std::string_view& bundleFile);

protected:
    const std::shared_ptr<T>& baseLoad(const std::string_view& name);
    bool baseUnload(const std::string_view& name)
    {
        return m_resources.erase(name.data()) == 1;
    }

    std::map<std::string, std::shared_ptr<T>> m_resources;
};