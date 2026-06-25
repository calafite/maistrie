#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace calafite {
    namespace dp {

        template<typename Container1, typename Container2>
        size_t longestCommonSubsequence(const Container1& sequence1, const Container2& sequence2) {
            size_t n = sequence1.size();
            size_t m = sequence2.size();
            if (n == 0 || m == 0) return 0;

            core::FastVector<size_t> previousRow(m + 1, 0);
            core::FastVector<size_t> currentRow(m + 1, 0);

            for (size_t i = 1; i <= n; ++i) {
                for (size_t j = 1; j <= m; ++j) {
                    if (sequence1[i - 1] == sequence2[j - 1]) {
                        currentRow[j] = previousRow[j - 1] + 1;
                    } else {
                        currentRow[j] = std::max(previousRow[j], currentRow[j - 1]);
                    }
                }
                previousRow = currentRow;
            }

            return previousRow[m];
        }

        template<typename Container1, typename Container2>
        auto longestCommonSubsequencePath(const Container1& sequence1, const Container2& sequence2) {
            using ValueType = std::remove_cv_t<std::remove_reference_t<decltype(sequence1[0])>>;
            
            size_t n = sequence1.size();
            size_t m = sequence2.size();
            if (n == 0 || m == 0) return core::FastVector<ValueType>();

            core::FastVector<core::FastVector<size_t>> dp(n + 1, core::FastVector<size_t>(m + 1, 0));

            for (size_t i = 1; i <= n; ++i) {
                for (size_t j = 1; j <= m; ++j) {
                    if (sequence1[i - 1] == sequence2[j - 1]) {
                        dp[i][j] = dp[i - 1][j - 1] + 1;
                    } else {
                        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                    }
                }
            }

            core::FastVector<ValueType> lcs;
            lcs.reserve(dp[n][m]);

            size_t i = n;
            size_t j = m;
            while (i > 0 && j > 0) {
                if (sequence1[i - 1] == sequence2[j - 1]) {
                    lcs.pushBack(sequence1[i - 1]);
                    --i;
                    --j;
                } else if (dp[i - 1][j] > dp[i][j - 1]) {
                    --i;
                } else {
                    --j;
                }
            }

            lcs.reverse();
            return lcs;
        }

    }
}
