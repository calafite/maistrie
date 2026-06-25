#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>

namespace calafite {
    namespace ds {

        inline constexpr size_t calculateLog2(size_t value) {
            size_t result = 0;
            while (value >>= 1) {
                ++result;
            }
            return result;
        }

        template<typename Type, typename Functor> struct SparseTable {
            size_t sizeValue;
            core::FastVector<core::FastVector<Type>> table;
            Functor combine;

            SparseTable(const core::FastVector<Type>& values, Functor combine)
                : sizeValue(values.size()), combine(std::move(combine)) {
                if (sizeValue > 0) {
                    size_t levels = calculateLog2(sizeValue) + 1;
                    table.assign(levels, core::FastVector<Type>(sizeValue));
                    table[0] = values;
                    
                    for (size_t level = 1; level < levels; ++level) {
                        for (size_t index = 0; index + (size_t(1) << level) <= sizeValue; ++index) {
                            table[level][index] = combine(table[level - 1][index], table[level - 1][index + (size_t(1) << (level - 1))]);
                        }
                    }
                }
            }

            Type query(size_t left, size_t right) const {
                assert(left <= right);
                assert(right < sizeValue);
                size_t level = calculateLog2(right - left + 1);
                return combine(table[level][left], table[level][right - (size_t(1) << level) + 1]);
            }
        };

    }
}
