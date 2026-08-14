#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>

namespace maistrie {
    namespace search {
        template <typename GraphType>
        inline core::FastVector<size_t> topologicalSort(const GraphType& adjacencyList) {
            size_t nodeCount = adjacencyList.size();
            core::FastVector<size_t> inDegree(nodeCount, 0);

            for (size_t u = 0; u < nodeCount; ++u) {
                for (size_t v : adjacencyList[u]) {
                    assert(v < nodeCount);
                    inDegree[v]++;
                }
            }

            core::FastVector<size_t> order;
            order.reserve(nodeCount);

            for (size_t i = 0; i < nodeCount; ++i) {
                if (inDegree[i] == 0) {
                    order.pushBack(i);
                }
            }

            for (size_t head = 0; head < order.size(); ++head) {
                size_t current = order[head];
                for (size_t neighbor : adjacencyList[current]) {
                    if (--inDegree[neighbor] == 0) {
                        order.pushBack(neighbor);
                    }
                }
            }

            if (order.size() != nodeCount) {
                return {};
            }

            return order;
        }
    } // namespace search
} // namespace maistrie
