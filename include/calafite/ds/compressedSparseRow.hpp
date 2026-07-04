#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace calafite {
    namespace ds {
        template <typename WeightType = void> class CSRGraph {
          public:
            static constexpr bool isWeighted = !std::is_same_v<WeightType, void>;
            using EdgeType = std::conditional_t<isWeighted, std::pair<size_t, WeightType>, size_t>;

            size_t nodeCount;
            size_t edgeCount;
            core::FastVector<size_t> offsets;
            core::FastVector<EdgeType> edges;

            CSRGraph() : nodeCount(0), edgeCount(0) {}

            template <typename Dummy = void>
            CSRGraph(size_t vertices, const core::FastVector<std::pair<size_t, size_t>>& edgeList,
                     bool bidirectional = true)
                : nodeCount(vertices) {
                static_assert(!isWeighted, "Use the weighted constructor for weighted graphs.");

                edgeCount = edgeList.size() * (bidirectional ? 2 : 1);
                offsets.assign(nodeCount + 1, 0);
                edges.assign(edgeCount, 0);

                for (const auto& edge : edgeList) {
                    offsets[edge.first]++;
                    if (bidirectional)
                        offsets[edge.second]++;
                }

                size_t currentOffset = 0;
                for (size_t i = 0; i < nodeCount; ++i) {
                    size_t degree = offsets[i];
                    offsets[i] = currentOffset;
                    currentOffset += degree;
                }
                offsets[nodeCount] = currentOffset;

                core::FastVector<size_t> current(offsets.begin(), nodeCount);
                for (const auto& edge : edgeList) {
                    edges[current[edge.first]++] = edge.second;
                    if (bidirectional) {
                        edges[current[edge.second]++] = edge.first;
                    }
                }
            }

            template <typename Dummy = void>
            CSRGraph(                     //
                size_t vertices,          //
                const core::FastVector<   //
                    std::pair<            //
                        std::pair<        //
                            size_t,       //
                            size_t        //
                            >,            //
                        WeightType>       //
                    >& edgeList,          //
                bool bidirectional = true //
                )
                : nodeCount(vertices) {
                static_assert(isWeighted, "Use the unweighted constructor for unweighted graphs.");

                edgeCount = edgeList.size() * (bidirectional ? 2 : 1);
                offsets.assign(nodeCount + 1, 0);

                if constexpr (std::is_trivially_constructible_v<WeightType>) {
                    edges.resize(edgeCount);
                } else {
                    edges.assign(edgeCount, {0, WeightType()});
                }

                for (const auto& edge : edgeList) {
                    offsets[edge.first.first]++;
                    if (bidirectional)
                        offsets[edge.first.second]++;
                }

                size_t currentOffset = 0;
                for (size_t i = 0; i < nodeCount; ++i) {
                    size_t degree = offsets[i];
                    offsets[i] = currentOffset;
                    currentOffset += degree;
                }
                offsets[nodeCount] = currentOffset;

                core::FastVector<size_t> current(offsets.begin(), nodeCount);
                for (const auto& edge : edgeList) {
                    size_t u = edge.first.first;
                    size_t v = edge.first.second;
                    WeightType w = edge.second;

                    edges[current[u]++] = {v, w};
                    if (bidirectional) {
                        edges[current[v]++] = {u, w};
                    }
                }
            }

            class AdjacencyList {
              public:
                const EdgeType* beginPtr;
                const EdgeType* endPtr;

                [[nodiscard]] inline const EdgeType* begin() const noexcept {
                    return beginPtr;
                }
                [[nodiscard]] inline const EdgeType* end() const noexcept {
                    return endPtr;
                }
                [[nodiscard]] inline size_t size() const noexcept {
                    return endPtr - beginPtr;
                }
            };

            [[nodiscard]] inline AdjacencyList operator[](size_t node) const noexcept {
                assert(node < nodeCount);
                return AdjacencyList{edges.data() + offsets[node],
                                     edges.data() + offsets[node + 1]};
            }

            [[nodiscard]] inline size_t getDegree(size_t node) const noexcept {
                assert(node < nodeCount);
                return offsets[node + 1] - offsets[node];
            }

            [[nodiscard]] inline size_t size() const noexcept {
                return nodeCount;
            }
        };
    } // namespace ds
} // namespace calafite
