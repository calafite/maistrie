#pragma once

#include "../io/io.hpp"
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace calafite {
    namespace mathematics {

        template<uint32_t Mod> class Modular {
            static_assert(Mod > 0, "Modulus must be strictly positive");
            
        private:
            uint32_t value;

        public:
            constexpr Modular() : value(0) {}

            template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
            constexpr Modular(Type initialValue) {
                using UnsignedType = std::make_unsigned_t<Type>;
                
                if constexpr (std::is_signed_v<Type>) {
                    if (initialValue >= 0) {
                        value = static_cast<uint32_t>(static_cast<UnsignedType>(initialValue) % Mod);
                    } else {
                        long long rem = static_cast<long long>(initialValue) % Mod;
                        value = static_cast<uint32_t>(rem == 0 ? 0 : rem + Mod);
                    }
                } else {
                    value = static_cast<uint32_t>(initialValue % Mod);
                }
            }

            constexpr uint32_t val() const noexcept { return value; }

            constexpr Modular operator+() const { return *this; }
            constexpr Modular operator-() const { return Modular() - *this; }

            constexpr Modular& operator+=(const Modular& other) {
                uint32_t res = value + other.value;
                value = res - (Mod & static_cast<uint32_t>(-(res >= Mod)));
                return *this;
            }

            constexpr Modular& operator-=(const Modular& other) {
                uint32_t res = value - other.value;
                value = res + (Mod & static_cast<uint32_t>(-(value < other.value)));
                return *this;
            }

            constexpr Modular& operator*=(const Modular& other) {
                value = static_cast<uint32_t>((static_cast<uint64_t>(value) * other.value) % Mod);
                return *this;
            }

            constexpr Modular& operator/=(const Modular& other) {
                return *this *= other.inverse();
            }

            constexpr Modular& operator++() {
                uint32_t res = value + 1;
                value = res - (Mod & static_cast<uint32_t>(-(res == Mod)));
                return *this;
            }

            constexpr Modular operator++(int) {
                Modular copy = *this;
                ++(*this);
                return copy;
            }

            constexpr Modular& operator--() {
                uint32_t res = value - 1;
                value = res + (Mod & static_cast<uint32_t>(-(value == 0)));
                return *this;
            }

            constexpr Modular operator--(int) {
                Modular copy = *this;
                --(*this);
                return copy;
            }

            constexpr Modular inverse() const {
                if constexpr (Mod <= 0x7FFFFFFF) {
                    int32_t a = value, b = Mod, u = 1, v = 0;
                    while (b != 0) {
                        int32_t t = a / b;
                        a -= t * b; std::swap(a, b);
                        u -= t * v; std::swap(u, v);
                    }
                    assert(a == 1 && "Modular inverse does not exist (not coprime).");
                    return Modular(u);
                } else {
                    int64_t a = value, b = Mod, u = 1, v = 0;
                    while (b != 0) {
                        int64_t t = a / b;
                        a -= t * b; std::swap(a, b);
                        u -= t * v; std::swap(u, v);
                    }
                    assert(a == 1 && "Modular inverse does not exist (not coprime).");
                    return Modular(u);
                }
            }

            constexpr Modular power(long long exponent) const {
                Modular result(1);
                Modular base(*this);
                if (exponent < 0) {
                    base = base.inverse();
                    exponent = -exponent;
                }
                uint64_t exp = exponent;
                while (exp > 0) {
                    if (exp & 1) result *= base;
                    base *= base;
                    exp >>= 1;
                }
                return result;
            }

            friend constexpr Modular operator+(Modular left, const Modular& right) { return left += right; }
            friend constexpr Modular operator-(Modular left, const Modular& right) { return left -= right; }
            friend constexpr Modular operator*(Modular left, const Modular& right) { return left *= right; }
            friend constexpr Modular operator/(Modular left, const Modular& right) { return left /= right; }
            friend constexpr bool operator==(const Modular& left, const Modular& right) { return left.value == right.value; }
            friend constexpr bool operator!=(const Modular& left, const Modular& right) { return left.value != right.value; }

            template<uint32_t M>
            friend inline io::Printer& operator<<(io::Printer& printer, const Modular<M>& number);

            template<uint32_t M>
            friend inline io::Scanner& operator>>(io::Scanner& scanner, Modular<M>& number);
        };

        template<uint32_t Mod>
        inline io::Printer& operator<<(io::Printer& printer, const Modular<Mod>& number) {
            return printer << number.value;
        }

        template<uint32_t Mod>
        inline io::Scanner& operator>>(io::Scanner& scanner, Modular<Mod>& number) {
            long long temporary;
            scanner >> temporary;
            number = Modular<Mod>(temporary);
            return scanner;
        }
    }
}
