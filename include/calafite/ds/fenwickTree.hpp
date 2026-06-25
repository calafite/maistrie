#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace ds {

        template<typename Type> struct FenwickTree {
            size_t sizeValue;
            core::FastVector<Type> tree;

            FenwickTree(size_t count) : sizeValue(count), tree(count + 1, Type(0)) {}

            FenwickTree(const core::FastVector<Type>& values) : sizeValue(values.size()), tree(values.size() + 1, Type(0)) {
                for (size_t index = 1; index <= sizeValue; ++index) {
                    tree[index] += values[index - 1];
                    size_t nextIndex = index + (index & -index);
                    if (nextIndex <= sizeValue) tree[nextIndex] += tree[index];
                }
            }

            Type query(size_t index) const {
                assert(index < sizeValue);
                Type sum = Type(0);
                for (size_t current = index + 1; current > 0; current -= current & -current) {
                    sum += tree[current];
                }
                return sum;
            }

            Type query(size_t left, size_t right) const {
                assert(left <= right);
                assert(right < sizeValue);
                if (left == 0) return query(right);
                return query(right) - query(left - 1);
            }

            void update(size_t index, Type value) {
                assert(index < sizeValue);
                for (size_t current = index + 1; current <= sizeValue; current += current & -current) {
                    tree[current] += value;
                }
            }

            void set(size_t index, Type value) {
                assert(index < sizeValue);
                update(index, value - query(index, index));
            }

            size_t kth(Type target) const {
                if (sizeValue == 0) return 0;
                size_t position = 0;
                size_t step = 1;
                
                while (step <= sizeValue / 2) {
                    step <<= 1;
                }
                
                for (; step > 0; step >>= 1) {
                    if (position + step <= sizeValue && tree[position + step] < target) {
                        position += step;
                        target -= tree[position];
                    }
                }
                
                return position;
            }
        };

    }
}
