#pragma once

#include "../core/fastVector.hpp"
#include "range.hpp"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace ds {

        template <typename Type, typename Functor> class SegmentTree {

          private:
            size_t sizeValue;
            core::FastVector<Type> tree;
            Type neutral;
            Functor combine;

          public:
            SegmentTree(size_t count, Type neutral, Functor combine)
                : sizeValue(count), tree(2 * count, neutral), neutral(neutral),
                  combine(std::move(combine)) {}

            template <typename Iter>
            SegmentTree(Iter first, Iter last, Type neutral, Functor combine)
                : SegmentTree(static_cast<size_t>(std::distance(first, last)), neutral,
                              std::move(combine)) {
                if (sizeValue > 0) {
                    size_t index = 0;
                    for (Iter it = first; it != last; ++it, ++index) {
                        tree[sizeValue + index] = *it;
                    }
                    for (size_t index = sizeValue - 1; index > 0; --index) {
                        tree[index] = combine(tree[index << 1], tree[index << 1 | 1]);
                    }
                }
            }

            template <typename Container, typename = std::enable_if_t<IsRange<Container>::value>>
            SegmentTree(const Container& values, Type neutral, Functor combine)
                : SegmentTree(std::begin(values), std::end(values), neutral, std::move(combine)) {}

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
                for (left += sizeValue, right += sizeValue + 1; left < right;
                     left >>= 1, right >>= 1) {
                    if (left & 1)
                        leftResult = combine(leftResult, tree[left++]);
                    if (right & 1)
                        rightResult = combine(tree[--right], rightResult);
                }
                return combine(leftResult, rightResult);
            }
        };

    } // namespace ds
} // namespace maistrie