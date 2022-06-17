#pragma once

#include <cstdint>

class Resource
{
public:
    virtual ~Resource() = default;

protected:
    explicit Resource(uint64_t resourceId) : m_resourceId(resourceId) {}

private:
    uint64_t m_resourceId;
};