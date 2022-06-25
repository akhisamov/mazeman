#pragma once

#include <string>

class Resource
{
public:
    virtual ~Resource() = default;

    [[nodiscard]] const std::string& getResourceId() const
    {
        return m_resourceId;
    }

protected:
    explicit Resource(const std::string_view& resourceId)
            :m_resourceId(resourceId)
    {
    }

private:
    std::string m_resourceId;
};