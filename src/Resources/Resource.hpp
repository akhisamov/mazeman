#pragma once

#include <string>

class Resource
{
public:
    virtual ~Resource() = default;

protected:
    explicit Resource(const std::string& resourceId)
            :m_resourceId(resourceId)
    {
    }

private:
    std::string m_resourceId;
};