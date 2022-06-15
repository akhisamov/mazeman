#pragma once

#include <cstdint>

class Resource
{
public:
    virtual ~Resource() = default;

    [[nodiscard]] uint64_t getResourceId() const { return m_resourceId; }

protected:
    explicit Resource(uint64_t resourceId) : m_resourceId(resourceId) {}

private:
    uint64_t m_resourceId;
};