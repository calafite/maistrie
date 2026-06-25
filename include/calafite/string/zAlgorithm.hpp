#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace calafite {
    namespace string {

        template<typename Container> core::FastVector<size_t> zFunction(const Container& string) {
            size_t sizeValue = string.size();
            core::FastVector<size_t> zTable(sizeValue, 0);
            if (sizeValue == 0) return zTable;

            const auto* data = string.data();
            size_t left = 0;
            size_t right = 0;
            for (size_t index = 1; index < sizeValue; ++index) {
                if (index <= right) {
                    zTable[index] = std::min(right - index + 1, zTable[index - left]);
                }
                while (index + zTable[index] < sizeValue && data[zTable[index]] == data[index + zTable[index]]) {
                    ++zTable[index];
                }
                if (index + zTable[index] > right + 1) {
                    left = index;
                    right = index + zTable[index] - 1;
                }
            }
            return zTable;
        }

        template<typename Container> struct ZAlgorithm {
            core::FastVector<size_t> zTable;
            size_t sizeValue;

            ZAlgorithm() : sizeValue(0) {}

            explicit ZAlgorithm(const Container& string) {
                zTable = zFunction(string);
                sizeValue = zTable.size();
            }

            inline const size_t& operator[](size_t index) const {
                assert(index < sizeValue);
                return zTable[index];
            }

            inline size_t size() const { return sizeValue; }
        };

        template<typename Container1, typename Container2>
        core::FastVector<size_t> zSearch(const Container1& text, const Container2& pattern) {
            core::FastVector<size_t> result;
            size_t textSize = text.size();
            size_t patternSize = pattern.size();
            if (patternSize == 0 || textSize == 0 || patternSize > textSize) return result;

            result.reserve(textSize / patternSize + 2);

            core::FastVector<size_t> patternZTable = zFunction(pattern);

            const auto* textData = text.data();
            const auto* patternData = pattern.data();

            size_t left = 0;
            size_t right = 0;

            for (size_t index = 0; index < textSize; ++index) {
                size_t matched = 0;

                if (index <= right) {
                    matched = std::min(right - index + 1, patternZTable[index - left]);
                }

                while (index + matched < textSize && matched < patternSize && textData[index + matched] == patternData[matched]) {
                    ++matched;
                }

                if (index + matched > right + 1) {
                    left = index;
                    right = index + matched - 1;
                }

                if (matched == patternSize) {
                    result.pushBack(index);
                }
            }

            return result;
        }

    }
}
