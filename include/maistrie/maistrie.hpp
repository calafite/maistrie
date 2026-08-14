#pragma once

// core
#include "core/arena.hpp"
#include "core/fastVector.hpp"
#include "core/iterator.hpp"
#include "core/ops.hpp"

// container
#include "container/circularBuffer.hpp"
#include "container/hashMap.hpp"
#include "container/priorityQueue.hpp"

// dynamic programming
#include "dp/knapsack.hpp"
#include "dp/lcs.hpp"
#include "dp/lis.hpp"
#include "dp/sos.hpp"

// data structures
#include "ds/cartesianTree.hpp"
#include "ds/compressedSparseRow.hpp"
#include "ds/compressor.hpp"
#include "ds/disjointSetUnion.hpp"
#include "ds/fenwickTree.hpp"
#include "ds/lazySegmentTree.hpp"
#include "ds/monotonicStack.hpp"
#include "ds/segmentTree.hpp"
#include "ds/sparseTable.hpp"
#include "ds/trie.hpp"

// custom io
#include "io/io.hpp"

// mathematics
#include "maths/fft.hpp"
#include "maths/linearSieve.hpp"
#include "maths/matrix.hpp"
#include "maths/mint.hpp"
#include "maths/sieve.hpp"

// geometry
#include "geometry/aosoa.hpp"
#include "geometry/line.hpp"
#include "geometry/point.hpp"

// search algorithms
#include "search/edgeEndpoint.hpp"
#include "search/dijkstra.hpp"
#include "search/graphBFS.hpp"
#include "search/graphDFS.hpp"
#include "search/stronglyConnected.hpp"
#include "search/topologicalSort.hpp"
#include "search/treeBFS.hpp"
#include "search/treeDFS.hpp"

// string algorithms
#include "string/KnuthMorrisPratt.hpp"
#include "string/zAlgorithm.hpp"

// utilities
#include "utils/fixPoint.hpp"
#include "utils/utils.hpp"

// macros
#define _all(x) (x).begin(), (x).end()
#define _debug(x) cerr << #x << " = " << x << "\n"
#define _upto(var, start, end) for (auto var = start; var < end; var++)
#define _downto(var, start, end) for (auto var = start; var > end; var--)
#define _foreach(element, collection) for (auto& element : collection)

#define MAISTRIE_SETUP                                                                             \
    using namespace std;                                                                           \
    using namespace maistrie;                                                                      \
    using ll = long long;                                                                          \
    using ull = unsigned long long;                                                                \
    using vi = ::maistrie::core::FastVector<int>;                                                  \
    using vll = ::maistrie::core::FastVector<ll>;                                                  \
    using pii = pair<int, int>;                                                                    \
    using pll = pair<ll, ll>;                                                                      \
    static constexpr int INF = 0x3f3f3f3f;                                                         \
    static constexpr int NINF = -0x3f3f3f3f;                                                       \
    static constexpr ll LLINF = 0x3f3f3f3f3f3f3f3fLL;                                              \
    static constexpr ll LNINF = -0x3f3f3f3f3f3f3f3fLL;                                             \
    using maistrie::io::in;                                                                        \
    using maistrie::io::out;                                                                       \
    using maistrie::io::print;                                                                     \
    using maistrie::io::println;                                                                   \
    using maistrie::io::read;
