#pragma once

#include <string>

namespace inari::strings {
    template <typename... Args>
    inline std::string format(const std::string_view& format, Args... args)
    {
        int size = std::snprintf(nullptr, 0, format.data(), args...) + 1;
        if (size <= 0) {
            return std::string(format);
        }
        std::string result;
        result.resize(size);
        std::snprintf(result.data(), size, format.data(), args...);
        return result;
    }
}; // namespace inari::strings
