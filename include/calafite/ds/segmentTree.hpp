#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>
#include <utility>

namespace calafite {
    namespace ds {

        template<typename Type, typename Functor> struct SegmentTree {
            size_t sizeValue;
            core::FastVector<Type> tree;
            Type neutral;
            Functor combine;

            SegmentTree(size_t count, Type neutral, Functor combine)
            : sizeValue(count),
            tree(2 * count, neutral),
            neutral(neutral),
            combine(std::move(combine)) {}

            SegmentTree(const core::FastVector<Type>& values, Type neutral, Functor combine)
            : sizeValue(values.size()),
            tree(2 * values.size(), neutral),
            neutral(neutral),
            combine(std::move(combine)) {
                if (sizeValue > 0) {
                    for (size_t index = 0; index < sizeValue; ++index) {
                        tree[sizeValue + index] = values[index];
                    }
                    for (size_t index = sizeValue - 1; index > 0; --index) {
                        tree[index] = combine(tree[index << 1], tree[index << 1 | 1]);
                    }
                }
            }

            void update(size_t index, Type value) {
                assert(index < sizeValue);
                size_t position = index + sizeValue;
                tree[position] = value;
                for (position >>= 1; position > 0; position >>= 1) {
                    tree[position] = combine(tree[position << 1], tree[position << 1 | 1]);
                }
            }

            Type query(size_t left, size_t right) const {
                assert(left <= right);
                assert(right < sizeValue);
                Type leftResult = neutral;
                Type rightResult = neutral;
                for (left += sizeValue, right += sizeValue + 1; left < right; left >>= 1, right >>= 1) {
                    if (left & 1) leftResult = combine(leftResult, tree[left++]);
                    if (right & 1) rightResult = combine(tree[--right], rightResult);
                }
                return combine(leftResult, rightResult);
            }
        };

    }
}
