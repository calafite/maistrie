#pragma once

#include "../core/fastVector.hpp"
#include "range.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace ds {

        inline constexpr size_t calculateLog2(size_t value) {
            size_t result = 0;
            while (value >>= 1) {
                ++result;
            }
            return result;
        }

        template <typename Type, typename Functor> class SparseTable {

          private:
            size_t sizeValue;
            core::FastVector<core::FastVector<Type>> table;
            Functor combine;

          public:
            template <typename Iter>
            SparseTable(Iter first, Iter last, Functor combine)
                : sizeValue(static_cast<size_t>(std::distance(first, last))),
                  combine(std::move(combine)) {
                if (sizeValue > 0) {
                    size_t levels = calculateLog2(sizeValue) + 1;
                    table.assign(levels, core::FastVector<Type>(sizeValue));

                    size_t index = 0;
                    for (Iter it = first; it != last; ++it, ++index) {
                        table[0][index] = *it;
                    }

                    for (size_t level = 1; level < levels; ++level) {
                        for (size_t index = 0; index + (size_t(1) << level) <= sizeValue; ++index) {
                            table[level][index] =
                                combine(table[level - 1][index],
                                        table[level - 1][index + (size_t(1) << (level - 1))]);
                        }
                    }
                }
            }

            template <typename Container, typename = std::enable_if_t<IsRange<Container>::value>>
            SparseTable(const Container& values, Functor combine)
                : SparseTable(std::begin(values), std::end(values), std::move(combine)) {}

            Type query(size_t left, size_t right) const {
                assert(left <= right);
                assert(right < sizeValue);
                size_t level = calculateLog2(right - left + 1);
                return combine(table[level][left], table[level][right - (size_t(1) << level) + 1]);
            }
        };

        template <typename Container, typename Functor>
        SparseTable(const Container&, Functor)
            -> SparseTable<typename Container::value_type, Functor>;

        template <typename Iter, typename Functor>
        SparseTable(Iter, Iter, Functor)
            -> SparseTable<typename std::iterator_traits<Iter>::value_type, Functor>;

    } // namespace ds
} // namespace maistrie
