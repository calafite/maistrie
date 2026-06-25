#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace ds {

        struct DisjointSetUnion {
            size_t componentCount;
            core::FastVector<size_t> parents;
            core::FastVector<size_t> sizes;

            explicit DisjointSetUnion(size_t count) : componentCount(count), parents(count), sizes(count, 1) {
                for (size_t index = 0; index < count; ++index) {
                    parents[index] = index;
                }
            }

            size_t find(size_t node) {
                assert(node < parents.size());
                if (parents[node] == node) return node;
                return parents[node] = find(parents[node]);
            }

            bool merge(size_t first, size_t second) {
                first = find(first);
                second = find(second);
                if (first == second) return false;
                if (sizes[first] < sizes[second]) std::swap(first, second);
                
                parents[second] = first;
                sizes[first] += sizes[second];
                --componentCount;
                
                return true;
            }

            bool same(size_t first, size_t second) {
                return find(first) == find(second);
            }

            size_t size(size_t node) {
                return sizes[find(node)];
            }
        };

    }
}
