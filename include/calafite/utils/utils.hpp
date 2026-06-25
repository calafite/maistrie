#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cstddef>

namespace calafite {
    namespace utility {

        template<typename Type>
        inline size_t lowerBound(const core::FastVector<Type>& vector, const Type& value) {
            return static_cast<size_t>(std::lower_bound(vector.begin(), vector.end(), value) - vector.begin());
        }

        template<typename Type>
        inline size_t upperBound(const core::FastVector<Type>& vector, const Type& value) {
            return static_cast<size_t>(std::upper_bound(vector.begin(), vector.end(), value) - vector.begin());
        }

    }
}
