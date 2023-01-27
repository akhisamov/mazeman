#pragma once

#include <string>

namespace inari {
    class IResource {
    public:
        IResource();
        virtual ~IResource() = default;

        const std::string& getUUID() const;

    private:
        std::string m_uuid;
    };
} // namespace inari