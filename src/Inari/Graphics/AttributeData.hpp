#pragma once

#include <array>
#include <string_view>

namespace inari {
struct AttributeData {
    std::string_view name;
    int size;
    int stride;
    bool normalized;
    size_t offset;

    constexpr AttributeData(const std::string_view& name,
                            int size,
                            int stride,
                            size_t offset = 0,
                            bool normalized = false)
        : name(name),
          size(size),
          stride(stride),
          normalized(normalized),
          offset(offset) {}
};

template <size_t N, typename AttributeMaker, typename... Ts>
constexpr std::enable_if_t<(sizeof...(Ts) == N), std::array<AttributeData, N>>
makeAttributes(AttributeMaker maker,
               const std::string_view* beg,
               const std::string_view* end,
               const Ts... xs) {
    return {maker(xs)...};
}

template <size_t N, typename AttributeMaker, typename... Ts>
constexpr std::enable_if_t<(sizeof...(Ts) < N), std::array<AttributeData, N>>
makeAttributes(AttributeMaker maker,
               const std::string_view* beg,
               const std::string_view* end,
               const Ts... xs) {
    return makeAttributes<N, AttributeMaker>(maker, beg + 1, end, xs..., *beg);
}

template <size_t N, typename AttributeMaker>
constexpr std::array<AttributeData, N> makeAttributes(
    AttributeMaker maker,
    const std::string_view (&names)[N]) {
    return makeAttributes<N, AttributeMaker>(maker, std::begin(names),
                                             std::end(names));
}
}  // namespace inari
