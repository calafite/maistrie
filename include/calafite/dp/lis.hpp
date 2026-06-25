#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>

namespace calafite {
    namespace dp {

        template<typename Container, typename Compare = std::less<>>
        size_t longestIncreasingSubsequence(const Container& sequence, Compare comp = Compare()) {
            if (sequence.size() == 0) return 0;
            core::FastVector<typename Container::value_type> dp;
            dp.reserve(sequence.size());
            
            for (const auto& value : sequence) {
                auto it = std::lower_bound(dp.begin(), dp.end(), value, comp);
                if (it == dp.end()) {
                    dp.pushBack(value);
                } else {
                    *it = value;
                }
            }
            return dp.size();
        }

        template<typename Container, typename Compare = std::less<>>
        core::FastVector<size_t> longestIncreasingSubsequenceIndices(const Container& sequence, Compare comp = Compare()) {
            size_t n = sequence.size();
            if (n == 0) return {};

            core::FastVector<size_t> parent(n, static_cast<size_t>(-1));
            core::FastVector<size_t> dpIndices;
            dpIndices.reserve(n);

            for (size_t i = 0; i < n; ++i) {
                const auto& value = sequence[i];
               
                auto it = std::lower_bound(dpIndices.begin(), dpIndices.end(), value,
                    [&](size_t idx, const auto& val) {
                        return comp(sequence[idx], val);
                    });
                
                size_t pos = static_cast<size_t>(it - dpIndices.begin());
                if (pos > 0) {
                    parent[i] = dpIndices[pos - 1];
                }
                
                if (it == dpIndices.end()) {
                    dpIndices.pushBack(i);
                } else {
                    *it = i;
                }
            }

            core::FastVector<size_t> lis;
            lis.reserve(dpIndices.size());
            for (size_t current = dpIndices.back(); current != static_cast<size_t>(-1); current = parent[current]) {
                lis.pushBack(current);
            }
            lis.reverse();
            return lis;
        }

    }
}
