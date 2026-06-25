#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace dp {

        template<typename Type>
        void sumOverSubsets(core::FastVector<Type>& dp, size_t bits) {
            assert(dp.size() == (1ull << bits));
            for (size_t i = 0; i < bits; ++i) {
                for (size_t mask = 0; mask < (1ull << bits); ++mask) {
                    if (mask & (1ull << i)) {
                        dp[mask] += dp[mask ^ (1ull << i)];
                    }
                }
            }
        }

        template<typename Type>
        void inverseSumOverSubsets(core::FastVector<Type>& dp, size_t bits) {
            assert(dp.size() == (1ull << bits));
            for (size_t i = 0; i < bits; ++i) {
                for (size_t mask = 0; mask < (1ull << bits); ++mask) {
                    if (mask & (1ull << i)) {
                        dp[mask] -= dp[mask ^ (1ull << i)];
                    }
                }
            }
        }

        template<typename Type>
        void sumOverSupersets(core::FastVector<Type>& dp, size_t bits) {
            assert(dp.size() == (1ull << bits));
            for (size_t i = 0; i < bits; ++i) {
                for (size_t mask = (1ull << bits) - 1; mask != static_cast<size_t>(-1); --mask) {
                    if (!(mask & (1ull << i))) {
                        dp[mask] += dp[mask ^ (1ull << i)];
                    }
                }
            }
        }

    }
}
