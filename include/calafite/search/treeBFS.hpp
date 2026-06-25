#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace search {

        struct TreeBreadthFirstSearch {
            static constexpr size_t unvisited = static_cast<size_t>(-1);

            core::FastVector<size_t> parents;
            core::FastVector<size_t> depths;
            core::FastVector<size_t> order;

            TreeBreadthFirstSearch(size_t root, const core::FastVector<core::FastVector<size_t>>& adjacencyList) {
                size_t nodeCount = adjacencyList.size();
                assert(root < nodeCount);

                parents.assign(nodeCount, unvisited);
                depths.assign(nodeCount, 0);
                order.reserve(nodeCount);

                order.pushBack(root);

                for (size_t head = 0; head < order.size(); ++head) {
                    size_t current = order[head];
                    for (size_t neighbor : adjacencyList[current]) {
                        if (neighbor != parents[current]) {
                            parents[neighbor] = current;
                            depths[neighbor] = depths[current] + 1;
                            order.pushBack(neighbor);
                        }
                    }
                }
            }

            core::FastVector<size_t> getPath(size_t target) const {
                assert(target < parents.size());

                if (parents[target] == unvisited && order.size() > 0 && order[0] != target) {
                    return {};
                }

                core::FastVector<size_t> path;
                for (size_t current = target; current != unvisited; current = parents[current]) {
                    path.pushBack(current);
                }
                
                path.reverse();
                return path;
            }
        };

    }
}
