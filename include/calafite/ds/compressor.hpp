#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace ds {

        template<typename Type> struct Compressor {
            core::FastVector<Type> values;

            Compressor() = default;

            explicit Compressor(const core::FastVector<Type>& initialValues) : values(initialValues) { build(); }

            inline void add(const Type& value) { values.pushBack(value); }

            inline void build() {
                values.sort();
                values.unique();
            }

            inline size_t get(const Type& value) const {
                return static_cast<size_t>(std::lower_bound(values.begin(), values.end(), value) - values.begin());
            }

            inline Type operator[](size_t index) const {
                assert(index < values.size());
                return values[index];
            }

            inline size_t size() const { return values.size(); }

            core::FastVector<size_t> compressArray(const core::FastVector<Type>& array) const {
                core::FastVector<size_t> result(array.size());
                for (size_t index = 0; index < array.size(); ++index) {
                    result[index] = get(array[index]);
                }
                return result;
            }
        };

    }
}
