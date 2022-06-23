#pragma once

#include <string>

class ResourceType
{
public:
    enum Type
    {
        NONE = 0,
        TEXTURE2D = 1,
        SHADER = 2
    };
    ResourceType()
            :m_type(NONE)
    {
    }
    ResourceType(Type type)
            :m_type(type)
    {
    }
    ResourceType(const std::string_view& type)
            :m_type(NONE)
    {
        if (type == "texture2d")
        {
            m_type = TEXTURE2D;
        }
        else if (type == "shader")
        {
            m_type = SHADER;
        }
    }

    explicit operator Type()
    {
        return m_type;
    }

    friend bool operator== (const ResourceType& t1, const Type& t2)
    {
        return t1.m_type == t2;
    }

    friend bool operator== (const ResourceType& t1, const ResourceType& t2)
    {
        return t1.m_type == t2.m_type;
    }

private:
    Type m_type;
};
