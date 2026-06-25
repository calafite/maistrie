#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace dp {

        template<typename WeightType, typename ValueType>
        ValueType zeroOneKnapsack(size_t capacity, const core::FastVector<WeightType>& weights, const core::FastVector<ValueType>& values) {
            assert(weights.size() == values.size());
            core::FastVector<ValueType> dp(capacity + 1, ValueType(0));
            
            for (size_t i = 0; i < weights.size(); ++i) {
                for (size_t w = capacity; w >= static_cast<size_t>(weights[i]); --w) {
                    dp[w] = std::max(dp[w], dp[w - weights[i]] + values[i]);
                }
            }
            return dp[capacity];
        }

        template<typename WeightType, typename ValueType>
        ValueType unboundedKnapsack(size_t capacity, const core::FastVector<WeightType>& weights, const core::FastVector<ValueType>& values) {
            assert(weights.size() == values.size());
            core::FastVector<ValueType> dp(capacity + 1, ValueType(0));
            
            for (size_t w = 0; w <= capacity; ++w) {
                for (size_t i = 0; i < weights.size(); ++i) {
                    if (w >= static_cast<size_t>(weights[i])) {
                        dp[w] = std::max(dp[w], dp[w - weights[i]] + values[i]);
                    }
                }
            }
            return dp[capacity];
        }

        template<typename WeightType, typename ValueType>
        ValueType boundedKnapsack(size_t capacity, const core::FastVector<WeightType>& weights, 
                                  const core::FastVector<ValueType>& values, const core::FastVector<size_t>& counts) {
            assert(weights.size() == values.size() && weights.size() == counts.size());
            
            core::FastVector<WeightType> splitWeights;
            core::FastVector<ValueType> splitValues;
            
            for (size_t i = 0; i < weights.size(); ++i) {
                size_t remaining = counts[i];
                for (size_t k = 1; k <= remaining; k <<= 1) {
                    splitWeights.pushBack(weights[i] * k);
                    splitValues.pushBack(values[i] * k);
                    remaining -= k;
                }
                if (remaining > 0) {
                    splitWeights.pushBack(weights[i] * remaining);
                    splitValues.pushBack(values[i] * remaining);
                }
            }
            
            return zeroOneKnapsack(capacity, splitWeights, splitValues);
        }

    }
}
