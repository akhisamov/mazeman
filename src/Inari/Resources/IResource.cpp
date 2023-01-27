#include "IResource.hpp"

#include "Inari/Utils/Random.hpp"

namespace inari {

    IResource::IResource()
        : m_uuid(random::generateUUID())
    {
    }

    const std::string& IResource::getUUID() const { return m_uuid; }

} // namespace inari