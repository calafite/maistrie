#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>

#if __cplusplus >= 202002L
    #include <bit>
#endif

namespace maistrie {
    namespace mathematics {
        constexpr std::size_t BITS_PER_BYTE = 8;

        template <typename Type> constexpr int countSetBits(Type value) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uValue = static_cast<UnsignedType>(value);

#if __cplusplus >= 202002L
            return std::popcount(uValue);
#else
            if constexpr (sizeof(UnsignedType) <= sizeof(unsigned int)) {
                return __builtin_popcount(uValue);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long)) {
                return __builtin_popcountl(uValue);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long long)) {
                return __builtin_popcountll(uValue);
            } else {
                return __builtin_popcountll(static_cast<unsigned long long>(uValue)) +
                    __builtin_popcountll(static_cast<unsigned long long>(uValue >> 64));
            }
#endif
        }

        template <typename Type> constexpr int countLeadingZeros(Type value) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uValue = static_cast<UnsignedType>(value);

            if (uValue == 0) {
                return sizeof(UnsignedType) * BITS_PER_BYTE;
            }

#if __cplusplus >= 202002L
            return std::countl_zero(uValue);
#else
            int bits = sizeof(UnsignedType) * BITS_PER_BYTE;
            if constexpr (sizeof(UnsignedType) <= sizeof(unsigned int)) {
                return __builtin_clz(uValue) - (sizeof(unsigned int) * BITS_PER_BYTE - bits);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long)) {
                return __builtin_clzl(uValue) - (sizeof(unsigned long) * BITS_PER_BYTE - bits);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long long)) {
                return __builtin_clzll(uValue) -
                    (sizeof(unsigned long long) * BITS_PER_BYTE - bits);
            } else {
                unsigned long long upper = static_cast<unsigned long long>(uValue >> 64);
                if (upper != 0) {
                    return __builtin_clzll(upper);
                }
                return 64 + __builtin_clzll(static_cast<unsigned long long>(uValue));
            }
#endif
        }

        template <typename Type> constexpr int countTrailingZeros(Type value) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uValue = static_cast<UnsignedType>(value);

            if (uValue == 0) {
                return sizeof(UnsignedType) * BITS_PER_BYTE;
            }

#if __cplusplus >= 202002L
            return std::countr_zero(uValue);
#else
            if constexpr (sizeof(UnsignedType) <= sizeof(unsigned int)) {
                return __builtin_ctz(uValue);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long)) {
                return __builtin_ctzl(uValue);
            } else if constexpr (sizeof(UnsignedType) <= sizeof(unsigned long long)) {
                return __builtin_ctzll(uValue);
            } else {
                unsigned long long lower = static_cast<unsigned long long>(uValue);
                if (lower != 0) {
                    return __builtin_ctzll(lower);
                }
                return 64 + __builtin_ctzll(static_cast<unsigned long long>(uValue >> 64));
            }
#endif
        }

        template <typename Type>
        [[nodiscard]] constexpr Type setBit(Type value, uint32_t bitPosition) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            return value | (static_cast<Type>(1) << bitPosition);
        }

        template <typename Type>
        [[nodiscard]] constexpr Type clearBit(Type value, uint32_t bitPosition) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            return value & ~(static_cast<Type>(1) << bitPosition);
        }

        template <typename Type>
        [[nodiscard]] constexpr Type toggleBit(Type value, uint32_t bitPosition) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            return value ^ (static_cast<Type>(1) << bitPosition);
        }

        template <typename Type>
        [[nodiscard]] constexpr bool checkBit(Type value, uint32_t bitPosition) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            return (value & (static_cast<Type>(1) << bitPosition)) != 0;
        }

        template <typename Type> [[nodiscard]] constexpr bool isPowerOfTwo(Type value) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uValue = static_cast<UnsignedType>(value);
            return uValue > 0 && (uValue & (uValue - 1)) == 0;
        }

        template <typename Type>
        [[nodiscard]] constexpr Type lowestSetBitValue(Type value) noexcept {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uValue = static_cast<UnsignedType>(value);
            return static_cast<Type>(uValue & -uValue);
        }

        template <typename Type = uint64_t> class Bitmask {
          public:
            static_assert(std::is_integral_v<Type> && std::is_unsigned_v<Type>,
                          "Bitmask requires an unsigned integral type");

          private:
            Type maskValue;

          public:
            constexpr Bitmask() noexcept : maskValue(0) {}
            constexpr Bitmask(Type initialMask) noexcept : maskValue(initialMask) {}

            constexpr Bitmask& set(uint32_t bitPosition) noexcept {
                maskValue |= (static_cast<Type>(1) << bitPosition);
                return *this;
            }

            constexpr Bitmask& clear(uint32_t bitPosition) noexcept {
                maskValue &= ~(static_cast<Type>(1) << bitPosition);
                return *this;
            }

            constexpr Bitmask& toggle(uint32_t bitPosition) noexcept {
                maskValue ^= (static_cast<Type>(1) << bitPosition);
                return *this;
            }

            [[nodiscard]] constexpr bool check(uint32_t bitPosition) const noexcept {
                return (maskValue & (static_cast<Type>(1) << bitPosition)) != 0;
            }

            [[nodiscard]] constexpr int count() const noexcept {
                return countSetBits(maskValue);
            }

            [[nodiscard]] constexpr int getLowestSetBit() const noexcept {
                return countTrailingZeros(maskValue);
            }

            [[nodiscard]] constexpr int getHighestSetBit() const noexcept {
                if (maskValue == 0) {
                    return -1;
                }
                return static_cast<int>(sizeof(Type) * BITS_PER_BYTE) - 1 -
                    countLeadingZeros(maskValue);
            }

            constexpr Bitmask& clearAll() noexcept {
                maskValue = 0;
                return *this;
            }

            constexpr Bitmask& setAll() noexcept {
                maskValue = ~static_cast<Type>(0);
                return *this;
            }

            [[nodiscard]] constexpr bool isEmpty() const noexcept {
                return maskValue == 0;
            }

            [[nodiscard]] constexpr Type getValue() const noexcept {
                return maskValue;
            }

            [[nodiscard]] constexpr Bitmask operator|(const Bitmask& other) const noexcept {
                return Bitmask(maskValue | other.maskValue);
            }

            constexpr Bitmask& operator|=(const Bitmask& other) noexcept {
                maskValue |= other.maskValue;
                return *this;
            }

            [[nodiscard]] constexpr Bitmask operator&(const Bitmask& other) const noexcept {
                return Bitmask(maskValue & other.maskValue);
            }

            constexpr Bitmask& operator&=(const Bitmask& other) noexcept {
                maskValue &= other.maskValue;
                return *this;
            }

            [[nodiscard]] constexpr Bitmask operator^(const Bitmask& other) const noexcept {
                return Bitmask(maskValue ^ other.maskValue);
            }

            constexpr Bitmask& operator^=(const Bitmask& other) noexcept {
                maskValue ^= other.maskValue;
                return *this;
            }

            [[nodiscard]] constexpr Bitmask operator~() const noexcept {
                return Bitmask(~maskValue);
            }

            [[nodiscard]] constexpr bool operator==(const Bitmask& other) const noexcept {
                return maskValue == other.maskValue;
            }

            [[nodiscard]] constexpr bool operator!=(const Bitmask& other) const noexcept {
                return maskValue != other.maskValue;
            }

            [[nodiscard]] constexpr bool isSubsetOf(const Bitmask& other) const noexcept {
                return (maskValue & other.maskValue) == maskValue;
            }

            [[nodiscard]] constexpr bool isSupersetOf(const Bitmask& other) const noexcept {
                return (maskValue & other.maskValue) == other.maskValue;
            }

            template <typename Func> constexpr void forEachSubmask(Func&& callback) const {
                Type submask = maskValue;
                do {
                    callback(Bitmask<Type>(submask));
                    if (submask == 0) {
                        break;
                    }
                    submask = (submask - 1) & maskValue;
                } while (true);
            }
        };

        template <typename Type, typename Func>
        constexpr void forEachSubmask(Type mask, Func&& callback) {
            static_assert(std::is_integral_v<Type>, "Must be an integral type");
            using UnsignedType = std::make_unsigned_t<Type>;
            UnsignedType uMask = static_cast<UnsignedType>(mask);
            UnsignedType submask = uMask;
            do {
                callback(static_cast<Type>(submask));
                if (submask == 0) {
                    break;
                }
                submask = (submask - 1) & uMask;
            } while (true);
        }

    } // namespace mathematics
} // namespace maistrie
