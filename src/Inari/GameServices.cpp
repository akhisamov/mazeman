#include "GameServices.h"

namespace inari {
    std::map<std::size_t, std::weak_ptr<IService>> GameServices::m_services;
}