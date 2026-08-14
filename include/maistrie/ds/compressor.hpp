#pragma once

#include "../core/fastVector.hpp"
#include "range.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace maistrie {
    namespace ds {

        template <typename Type> class Compressor {

          private:
            core::FastVector<Type> values;

          public:
            Compressor() = default;

            template <typename Iter>
            Compressor(Iter first, Iter last)
                : values(static_cast<size_t>(std::distance(first, last))) {
                size_t index = 0;
                for (Iter it = first; it != last; ++it, ++index) {
                    values[index] = *it;
                }
                build();
            }

            template <typename Container, typename = std::enable_if_t<IsRange<Container>::value>>
            explicit Compressor(const Container& initialValues)
                : Compressor(std::begin(initialValues), std::end(initialValues)) {}

            inline void add(const Type& value) {
                values.pushBack(value);
            }

            inline void build() {
                values.sort();
                values.unique();
            }

            inline size_t get(const Type& value) const {
                return static_cast<size_t>( //
                    std::lower_bound(       //
                        values.begin(),     //
                        values.end(), value //
                        ) -
                    values.begin() //
                );
            }

            inline Type operator[](size_t index) const {
                assert(index < values.size());
                return values[index];
            }

            inline size_t size() const {
                return values.size();
            }

            template <typename Container>
            core::FastVector<size_t> compressArray(const Container& array) const {
                core::FastVector<size_t> result(static_cast<size_t>(std::size(array)));
                size_t index = 0;
                for (auto it = std::begin(array); it != std::end(array); ++it, ++index) {
                    result[index] = get(*it);
                }
                return result;
            }
        };

        template <typename Container>
        Compressor(const Container&) -> Compressor<typename Container::value_type>;

        template <typename Iter>
        Compressor(Iter, Iter) -> Compressor<typename std::iterator_traits<Iter>::value_type>;

    } // namespace ds
} // namespace maistrie
