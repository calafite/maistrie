#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace calafite {
    namespace search {

        struct GraphDepthFirstSearch {
            static constexpr size_t unvisited = static_cast<size_t>(-1);

            core::FastVector<size_t> parents;
            core::FastVector<size_t> entryTimes;
            core::FastVector<size_t> exitTimes;
            core::FastVector<uint8_t> states; 
            size_t timer;
            bool hasCycle;

            GraphDepthFirstSearch(size_t start, const core::FastVector<core::FastVector<size_t>>& adjacencyList, bool recordPaths = false) {
                size_t nodeCount = adjacencyList.size();
                assert(start < nodeCount);

                states.assign(nodeCount, 0);
                entryTimes.assign(nodeCount, unvisited);
                exitTimes.assign(nodeCount, unvisited);
                if (recordPaths) {
                    parents.assign(nodeCount, unvisited);
                }
                timer = 0;
                hasCycle = false;

                performDepthFirstSearch(start, unvisited, adjacencyList, recordPaths);
            }

            GraphDepthFirstSearch(const core::FastVector<size_t>& starts, const core::FastVector<core::FastVector<size_t>>& adjacencyList, bool recordPaths = false) {
                size_t nodeCount = adjacencyList.size();

                states.assign(nodeCount, 0);
                entryTimes.assign(nodeCount, unvisited);
                exitTimes.assign(nodeCount, unvisited);
                if (recordPaths) {
                    parents.assign(nodeCount, unvisited);
                }
                timer = 0;
                hasCycle = false;

                for (size_t start : starts) {
                    assert(start < nodeCount);
                    if (states[start] == 0) {
                        performDepthFirstSearch(start, unvisited, adjacencyList, recordPaths);
                    }
                }
            }

            core::FastVector<size_t> getPath(size_t target) const {
                assert(!parents.empty() && "Paths were not tracked. Initialize the GraphDepthFirstSearch struct with recordPaths = true.");
                assert(target < states.size());

                if (states[target] == 0) return {};

                core::FastVector<size_t> path;
                for (size_t current = target; current != unvisited; current = parents[current]) {
                    path.pushBack(current);
                }
                
                path.reverse();
                return path;
            }

        private:
            void performDepthFirstSearch(size_t current, size_t parent, const core::FastVector<core::FastVector<size_t>>& adjacencyList, bool recordPaths) {
                states[current] = 1;
                entryTimes[current] = timer++;
                if (recordPaths) parents[current] = parent;

                for (size_t neighbor : adjacencyList[current]) {
                    if (states[neighbor] == 0) {
                        performDepthFirstSearch(neighbor, current, adjacencyList, recordPaths);
                    } else if (states[neighbor] == 1) {
                        hasCycle = true;
                    }
                }
                
                states[current] = 2;
                exitTimes[current] = timer;
            }
        };

    }
}
