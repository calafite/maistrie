#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <bit>

namespace calafite {
    namespace core {
        namespace operations {

            class FastModulo32 {
            private:
                uint32_t mod;
                uint64_t m;

            public:
                constexpr explicit FastModulo32(uint32_t modulus) 
                    : mod(modulus), m(modulus == 0 ? 0 : (static_cast<uint64_t>(-1) / modulus + 1)) {}

                constexpr uint32_t operator()(uint64_t value) const {
                #if defined(__SIZEOF_INT128__)
                    uint64_t q = (static_cast<unsigned __int128>(value) * m) >> 64;
                    uint32_t r = static_cast<uint32_t>(value - q * mod);
                    return r >= mod ? r - mod : r;
                #else
                    return value % mod;
                #endif
                }
            };

            namespace epsilon {

                inline constexpr double defaultEpsilon = 1e-9;

                class Equal {
                private:
                    double epsilon;

                public:
                    constexpr Equal(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return std::abs(left - right) < epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return std::abs(left - right) < epsilon;
                        };
                    }

                    constexpr Equal withEpsilon(double newEpsilon) const {
                        return Equal{newEpsilon};
                    }
                };
                inline constexpr Equal equal{defaultEpsilon};

                class NotEqual {
                private:
                    double epsilon;

                public:
                    constexpr NotEqual(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return std::abs(left - right) >= epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return std::abs(left - right) >= epsilon;
                        };
                    }

                    constexpr NotEqual withEpsilon(double newEpsilon) const {
                        return NotEqual{newEpsilon};
                    }
                };
                inline constexpr NotEqual notEqual{defaultEpsilon};

                class LessThan {
                private:
                    double epsilon;

                public:
                    constexpr LessThan(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return left < right - epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return left < right - epsilon;
                        };
                    }

                    constexpr LessThan withEpsilon(double newEpsilon) const {
                        return LessThan{newEpsilon};
                    }
                };
                inline constexpr LessThan lessThan{defaultEpsilon};

                class GreaterThan {
                private:
                    double epsilon;

                public:
                    constexpr GreaterThan(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return left > right + epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return left > right + epsilon;
                        };
                    }

                    constexpr GreaterThan withEpsilon(double newEpsilon) const {
                        return GreaterThan{newEpsilon};
                    }
                };
                inline constexpr GreaterThan greaterThan{defaultEpsilon};

                class LessThanOrEqual {
                private:
                    double epsilon;

                public:
                    constexpr LessThanOrEqual(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return left < right + epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return left < right + epsilon;
                        };
                    }

                    constexpr LessThanOrEqual withEpsilon(double newEpsilon) const {
                        return LessThanOrEqual{newEpsilon};
                    }
                };
                inline constexpr LessThanOrEqual lessThanOrEqual{defaultEpsilon};

                class GreaterThanOrEqual {
                private:
                    double epsilon;

                public:
                    constexpr GreaterThanOrEqual(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    template<typename LeftType, typename RightType>
                    constexpr bool operator()(const LeftType& left, const RightType& right) const {
                        return left > right - epsilon;
                    }

                    template<typename RightType>
                    constexpr auto operator()(const RightType& right) const {
                        return [right, epsilon = epsilon](const auto& left) {
                            return left > right - epsilon;
                        };
                    }

                    constexpr GreaterThanOrEqual withEpsilon(double newEpsilon) const {
                        return GreaterThanOrEqual{newEpsilon};
                    }
                };
                inline constexpr GreaterThanOrEqual greaterThanOrEqual{defaultEpsilon};

                class IsZero {
                private:
                    double epsilon;

                public:
                    constexpr IsZero(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    constexpr bool operator()(double value) const {
                        return std::abs(value) < epsilon;
                    }

                    constexpr IsZero withEpsilon(double newEpsilon) const {
                        return IsZero{newEpsilon};
                    }
                };
                inline constexpr IsZero isZero{defaultEpsilon};

                class IsPositive {
                private:
                    double epsilon;

                public:
                    constexpr IsPositive(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    constexpr bool operator()(double value) const {
                        return value > epsilon;
                    }

                    constexpr IsPositive withEpsilon(double newEpsilon) const {
                        return IsPositive{newEpsilon};
                    }
                };
                inline constexpr IsPositive isPositive{defaultEpsilon};

                class IsNegative {
                private:
                    double epsilon;

                public:
                    constexpr IsNegative(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    constexpr bool operator()(double value) const {
                        return value < -epsilon;
                    }

                    constexpr IsNegative withEpsilon(double newEpsilon) const {
                        return IsNegative{newEpsilon};
                    }
                };
                inline constexpr IsNegative isNegative{defaultEpsilon};

            }

            class Add {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left + right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left + right;
                    };
                }
            };
            inline constexpr Add add{};

            class Subtract {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left - right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left - right;
                    };
                }
            };
            inline constexpr Subtract subtract{};

            class Multiply {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left * right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left * right;
                    };
                }
            };
            inline constexpr Multiply multiply{};

            class Divide {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    assert(right != 0);
                    return left / right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    assert(right != 0);
                    return [right](const auto& left) {
                        return left / right;
                    };
                }
            };
            inline constexpr Divide divide{};

            class Modulo {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    assert(right != 0);
                    return left % right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    assert(right != 0);
                    return [right](const auto& left) {
                        return left % right;
                    };
                }
            };
            inline constexpr Modulo modulo{};

            class Equal {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left == right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left == right;
                    };
                }
            };
            inline constexpr Equal equal{};

            class NotEqual {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left != right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left != right;
                    };
                }
            };
            inline constexpr NotEqual notEqual{};

            class LessThan {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left < right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left < right;
                    };
                }
            };
            inline constexpr LessThan lessThan{};

            class GreaterThan {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left > right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left > right;
                    };
                }
            };
            inline constexpr GreaterThan greaterThan{};

            class LessThanOrEqual {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left <= right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left <= right;
                    };
                }
            };
            inline constexpr LessThanOrEqual lessThanOrEqual{};

            class GreaterThanOrEqual {
            public:
                template<typename LeftType, typename RightType>
                constexpr bool operator()(const LeftType& left, const RightType& right) const {
                    return left >= right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left >= right;
                    };
                }
            };
            inline constexpr GreaterThanOrEqual greaterThanOrEqual{};

            class BitwiseAnd {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left & right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left & right;
                    };
                }
            };
            inline constexpr BitwiseAnd bitwiseAnd{};

            class BitwiseOr {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left | right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left | right;
                    };
                }
            };
            inline constexpr BitwiseOr bitwiseOr{};

            class BitwiseXor {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left ^ right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return left ^ right;
                    };
                }
            };
            inline constexpr BitwiseXor bitwiseXor{};

            class Minimum {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left < right ? left : right;
                }
            };
            inline constexpr Minimum minimum{};

            class Maximum {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left > right ? left : right;
                }
            };
            inline constexpr Maximum maximum{};

            class GreatestCommonDivisor {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(LeftType left, RightType right) const {
                    using T = std::make_unsigned_t<std::common_type_t<LeftType, RightType>>;
                    T u = left < 0 ? -left : left;
                    T v = right < 0 ? -right : right;
                    if (u == 0) return v;
                    if (v == 0) return u;
                    
                    int shift = std::countr_zero(u | v);
                    u >>= std::countr_zero(u);
                    
                    do {
                        v >>= std::countr_zero(v);
                        if (u > v) std::swap(u, v);
                        v -= u;
                    } while (v != 0);
                    
                    return u << shift;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right, this](const auto& left) {
                        return (*this)(left, right);
                    };
                }
            };
            inline constexpr GreatestCommonDivisor greatestCommonDivisor{};

            class LeastCommonMultiple {
            public:
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    if (left == 0 || right == 0) return 0;
                    return (left / greatestCommonDivisor(left, right)) * right;
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right, this](const auto& left) {
                        return (*this)(left, right);
                    };
                }
            };
            inline constexpr LeastCommonMultiple leastCommonMultiple{};

            class IsEven {
            public:
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return (value & 1) == 0;
                }
            };
            inline constexpr IsEven isEven{};

            class IsOdd {
            public:
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return (value & 1) != 0;
                }
            };
            inline constexpr IsOdd isOdd{};

            class IsPositive {
            public:
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value > 0;
                }
            };
            inline constexpr IsPositive isPositive{};

            class IsNegative {
            public:
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value < 0;
                }
            };
            inline constexpr IsNegative isNegative{};

            class IsZero {
            public:
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value == 0;
                }
            };
            inline constexpr IsZero isZero{};

        }
    }
}
