#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace search {
        template <typename EdgeType> constexpr size_t getEndpoint(const EdgeType& edge) {
            if constexpr (std::is_integral_v<EdgeType>) {
                return static_cast<size_t>(edge);
            } else {
                return static_cast<size_t>(edge.first);
            }
        }

        class StronglyConnectedComponents {
          public:
          public:
            size_t componentCount;
            core::FastVector<size_t> componentId;
            core::FastVector<core::FastVector<size_t>> components;

            template <typename GraphType>
            explicit StronglyConnectedComponents(const GraphType& adjacencyList) {
                size_t nodeCount = adjacencyList.size();
                componentId.assign(nodeCount, static_cast<size_t>(-1));
                componentCount = 0;

                entryTimes.assign(nodeCount, 0);
                lowLinks.assign(nodeCount, 0);
                onStack.assign(nodeCount, 0);
                nodeStack.reserve(nodeCount);
                timer = 1;

                for (size_t index = 0; index < nodeCount; ++index) {
                    if (entryTimes[index] == 0) {
                        performTarjan(index, adjacencyList);
                    }
                }

                entryTimes.clear();
                lowLinks.clear();
                onStack.clear();
                nodeStack.clear();
            }

            template <typename GraphType>
            [[nodiscard]] core::FastVector<core::FastVector<size_t>>
            buildCondensationGraph(const GraphType& adjacencyList) const {

                core::FastVector<core::FastVector<size_t>> dag(componentCount);
                size_t nodeCount = adjacencyList.size();

                for (size_t u = 0; u < nodeCount; ++u) {
                    size_t uComponent = componentId[u];
                    for (const auto& edge : adjacencyList[u]) {
                        size_t v = getEndpoint(edge);
                        size_t vComponent = componentId[v];
                        if (uComponent != vComponent) {
                            dag[uComponent].pushBack(vComponent);
                        }
                    }
                }

                for (size_t i = 0; i < componentCount; ++i) {
                    dag[i].sort();
                    dag[i].unique();
                }

                return dag;
            }

          private:
            core::FastVector<size_t> entryTimes;
            core::FastVector<size_t> lowLinks;
            core::FastVector<uint8_t> onStack;
            core::FastVector<size_t> nodeStack;
            size_t timer;

            template <typename GraphType>
            void performTarjan(size_t current, const GraphType& adjacencyList) {
                entryTimes[current] = lowLinks[current] = timer++;
                nodeStack.pushBack(current);
                onStack[current] = 1;

                for (const auto& edge : adjacencyList[current]) {
                    size_t neighbor = getEndpoint(edge);

                    if (entryTimes[neighbor] == 0) {
                        performTarjan(neighbor, adjacencyList);
                        lowLinks[current] = std::min(lowLinks[current], lowLinks[neighbor]);
                    } else if (onStack[neighbor] == 1) {
                        lowLinks[current] = std::min(lowLinks[current], entryTimes[neighbor]);
                    }
                }

                if (lowLinks[current] == entryTimes[current]) {
                    core::FastVector<size_t> currentComponent;
                    size_t node;
                    do {
                        node = nodeStack.back();
                        nodeStack.popBack();
                        onStack[node] = 0;
                        componentId[node] = componentCount;
                        currentComponent.pushBack(node);
                    } while (current != node);

                    components.pushBack(std::move(currentComponent));
                    componentCount++;
                }
            }
        };
    } // namespace search
} // namespace maistrie
