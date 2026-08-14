#pragma once

#include <cstddef>
#include <type_traits>

namespace maistrie {
    namespace search {
        template <typename EdgeType> constexpr size_t getEndpoint(const EdgeType& edge) {
            if constexpr (std::is_integral_v<EdgeType>) {
                return static_cast<size_t>(edge);
            } else {
                return static_cast<size_t>(edge.first);
            }
        }
    } // namespace search
} // namespace maistrie
