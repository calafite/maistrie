#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace search {

        struct GraphBreadthFirstSearch {
            static constexpr size_t unvisited = static_cast<size_t>(-1);

            core::FastVector<size_t> distances;
            core::FastVector<size_t> parents;

            GraphBreadthFirstSearch(size_t start, const core::FastVector<core::FastVector<size_t>>& adjacencyList, bool recordPaths = false) {
                size_t nodeCount = adjacencyList.size();
                assert(start < nodeCount);

                distances.assign(nodeCount, unvisited);
                if (recordPaths) {
                    parents.assign(nodeCount, unvisited);
                }

                core::FastVector<size_t> queue;
                queue.reserve(nodeCount);

                distances[start] = 0;
                queue.pushBack(start);

                for (size_t head = 0; head < queue.size(); ++head) {
                    size_t current = queue[head];
                    for (size_t neighbor : adjacencyList[current]) {
                        if (distances[neighbor] == unvisited) {
                            distances[neighbor] = distances[current] + 1;
                            if (recordPaths) parents[neighbor] = current;
                            queue.pushBack(neighbor);
                        }
                    }
                }
            }

            GraphBreadthFirstSearch(const core::FastVector<size_t>& starts, const core::FastVector<core::FastVector<size_t>>& adjacencyList, bool recordPaths = false) {
                size_t nodeCount = adjacencyList.size();
                distances.assign(nodeCount, unvisited);
                if (recordPaths) {
                    parents.assign(nodeCount, unvisited);
                }

                core::FastVector<size_t> queue;
                queue.reserve(nodeCount);

                for (size_t start : starts) {
                    assert(start < nodeCount);
                    if (distances[start] == unvisited) {
                        distances[start] = 0;
                        queue.pushBack(start);
                    }
                }

                for (size_t head = 0; head < queue.size(); ++head) {
                    size_t current = queue[head];
                    for (size_t neighbor : adjacencyList[current]) {
                        if (distances[neighbor] == unvisited) {
                            distances[neighbor] = distances[current] + 1;
                            if (recordPaths) parents[neighbor] = current;
                            queue.pushBack(neighbor);
                        }
                    }
                }
            }

            core::FastVector<size_t> getPath(size_t target) const {
                assert(!parents.empty() && "Paths were not tracked. Initialize the GraphBreadthFirstSearch struct with recordPaths = true.");
                assert(target < distances.size());

                if (distances[target] == unvisited) return {};

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
