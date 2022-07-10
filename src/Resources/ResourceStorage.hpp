#pragma once

#include <map>
#include <memory>
#include <string>

template <class T>
class ResourceStorage
{
public:
    virtual ~ResourceStorage()
    {
        m_resources.clear();
    }

protected:
    std::map<std::string_view, std::shared_ptr<T>>& getResources() { return m_resources; }

private:
    std::map<std::string_view, std::shared_ptr<T>> m_resources;
};
