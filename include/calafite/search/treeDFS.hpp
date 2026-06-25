#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace calafite {
    namespace search {

        struct TreeDepthFirstSearch {
            static constexpr size_t unvisited = static_cast<size_t>(-1);

            core::FastVector<size_t> parents;
            core::FastVector<size_t> depths;
            core::FastVector<size_t> subtreeSizes;
            core::FastVector<size_t> entryTimes;
            core::FastVector<size_t> exitTimes;
            size_t timer;

            TreeDepthFirstSearch(size_t root, const core::FastVector<core::FastVector<size_t>>& adjacencyList) {
                size_t nodeCount = adjacencyList.size();
                parents.assign(nodeCount, unvisited);
                depths.assign(nodeCount, 0);
                subtreeSizes.assign(nodeCount, 1);
                entryTimes.assign(nodeCount, unvisited);
                exitTimes.assign(nodeCount, unvisited);
                timer = 0;

                performDepthFirstSearch(root, unvisited, adjacencyList);
            }

            bool isAncestor(size_t first, size_t second) const {
                assert(first < entryTimes.size());
                assert(second < entryTimes.size());
                return entryTimes[first] <= entryTimes[second] && entryTimes[second] < exitTimes[first];
            }

        private:
            void performDepthFirstSearch(size_t current, size_t parent, const core::FastVector<core::FastVector<size_t>>& adjacencyList) {
                parents[current] = parent;
                entryTimes[current] = timer++;

                for (size_t neighbor : adjacencyList[current]) {
                    if (neighbor != parent) {
                        depths[neighbor] = depths[current] + 1;
                        performDepthFirstSearch(neighbor, current, adjacencyList);
                        subtreeSizes[current] += subtreeSizes[neighbor];
                    }
                }
                
                exitTimes[current] = timer;
            }
        };

        struct TopologicalSorter {
            const core::FastVector<core::FastVector<size_t>>& adjacencyList;
            core::FastVector<size_t>& order;
            core::FastVector<uint8_t>& states;
            bool& hasCycle;

            void visit(size_t current) {
                states[current] = 1;
                for (size_t neighbor : adjacencyList[current]) {
                    if (states[neighbor] == 0) {
                        visit(neighbor);
                    } else if (states[neighbor] == 1) {
                        hasCycle = true;
                    }
                }
                states[current] = 2;
                order.pushBack(current);
            }
        };

        inline core::FastVector<size_t> topologicalSort(const core::FastVector<core::FastVector<size_t>>& adjacencyList) {
            size_t nodeCount = adjacencyList.size();
            core::FastVector<size_t> order;
            order.reserve(nodeCount);

            core::FastVector<uint8_t> states(nodeCount, 0);
            bool hasCycle = false;

            TopologicalSorter sorter{adjacencyList, order, states, hasCycle};

            for (size_t index = 0; index < nodeCount; ++index) {
                if (states[index] == 0 && !hasCycle) sorter.visit(index);
            }

            if (hasCycle) return {};

            order.reverse();
            return order;
        }

    }
}
