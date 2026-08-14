#pragma once

#include "../core/fastVector.hpp"
#include "point.hpp"
#include <cassert>
#include <cstddef>

namespace maistrie {
    namespace geometry {
        template <typename Type>
        constexpr size_t ChunkSize = (64 / sizeof(Type) > 0 ? 64 / sizeof(Type) : 1);

        template <typename Type, size_t CSize = ChunkSize<Type>> class PointAoSoA {
          public:
            class alignas(64) Block {
              public:
                Type x[CSize];
                Type y[CSize];

                constexpr Block() noexcept : x{}, y{} {}
            };

            class Reference {
              public:
                Type& x;
                Type& y;

                constexpr Reference& operator=(Point<Type> point) noexcept {
                    x = point.x;
                    y = point.y;
                    return *this;
                }

                constexpr Reference& operator=(const Reference& other) noexcept {
                    x = other.x;
                    y = other.y;
                    return *this;
                }

                [[nodiscard]] constexpr operator Point<Type>() const noexcept {
                    return Point<Type>(x, y);
                }

                constexpr Reference& operator+=(Point<Type> point) noexcept {
                    x += point.x;
                    y += point.y;
                    return *this;
                }

                constexpr Reference& operator-=(Point<Type> point) noexcept {
                    x -= point.x;
                    y -= point.y;
                    return *this;
                }
            };

            class ConstReference {
              public:
                const Type& x;
                const Type& y;

                [[nodiscard]] constexpr operator Point<Type>() const noexcept {
                    return Point<Type>(x, y);
                }
            };

            core::FastVector<Block> blocks;
            size_t sizeValue;

            PointAoSoA() noexcept : sizeValue(0) {}

            explicit PointAoSoA(size_t initialSize) : sizeValue(0) {
                resize(initialSize);
            }

            inline void pushBack(Point<Type> point) {
                size_t blockIndex = sizeValue / CSize;
                size_t elementIndex = sizeValue % CSize;
                if (blockIndex >= blocks.size()) {
                    blocks.emplaceBack();
                }
                blocks[blockIndex].x[elementIndex] = point.x;
                blocks[blockIndex].y[elementIndex] = point.y;
                ++sizeValue;
            }

            inline void popBack() noexcept {
                assert(sizeValue > 0);
                --sizeValue;
                if (sizeValue > 0 && sizeValue % CSize == 0) {
                    blocks.popBack();
                }
            }

            [[nodiscard]] inline Reference operator[](size_t index) noexcept {
                assert(index < sizeValue);
                size_t blockIndex = index / CSize;
                size_t elementIndex = index % CSize;
                return Reference{blocks[blockIndex].x[elementIndex],
                                 blocks[blockIndex].y[elementIndex]};
            }

            [[nodiscard]] inline ConstReference operator[](size_t index) const noexcept {
                assert(index < sizeValue);
                size_t blockIndex = index / CSize;
                size_t elementIndex = index % CSize;
                return ConstReference{blocks[blockIndex].x[elementIndex],
                                      blocks[blockIndex].y[elementIndex]};
            }

            [[nodiscard]] inline size_t size() const noexcept {
                return sizeValue;
            }

            [[nodiscard]] inline bool empty() const noexcept {
                return sizeValue == 0;
            }

            [[nodiscard]] inline size_t capacity() const noexcept {
                return blocks.capacity() * CSize;
            }

            [[nodiscard]] inline size_t blockCount() const noexcept {
                return blocks.size();
            }

            [[nodiscard]] inline Block& block(size_t index) noexcept {
                assert(index < blocks.size());
                return blocks[index];
            }

            [[nodiscard]] inline const Block& block(size_t index) const noexcept {
                assert(index < blocks.size());
                return blocks[index];
            }

            inline void clear() noexcept {
                blocks.clear();
                sizeValue = 0;
            }

            inline void reserve(size_t newCapacity) {
                size_t blockCapacity = (newCapacity + CSize - 1) / CSize;
                blocks.reserve(blockCapacity);
            }

            inline void resize(size_t newSize) {
                size_t newBlockCount = (newSize + CSize - 1) / CSize;
                blocks.resize(newBlockCount);
                sizeValue = newSize;
            }
        };

    } // namespace geometry
} // namespace maistrie
