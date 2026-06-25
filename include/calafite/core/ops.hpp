#pragma once

#include <cassert>
#include <cmath>
#include <numeric>

namespace calafite {
    namespace core {
        namespace operations {

            namespace epsilon {

                inline constexpr double defaultEpsilon = 1e-9;

                struct Equal {
                    double epsilon;
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

                struct NotEqual {
                    double epsilon;
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

                struct LessThan {
                    double epsilon;
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

                struct GreaterThan {
                    double epsilon;
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

                struct LessThanOrEqual {
                    double epsilon;
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

                struct GreaterThanOrEqual {
                    double epsilon;
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

                struct IsZero {
                    double epsilon;
                    constexpr IsZero(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    constexpr bool operator()(double value) const {
                        return std::abs(value) < epsilon;
                    }

                    constexpr IsZero withEpsilon(double newEpsilon) const {
                        return IsZero{newEpsilon};
                    }
                };
                inline constexpr IsZero isZero{defaultEpsilon};

                struct IsPositive {
                    double epsilon;
                    constexpr IsPositive(double epsilon = defaultEpsilon) : epsilon(epsilon) {}

                    constexpr bool operator()(double value) const {
                        return value > epsilon;
                    }

                    constexpr IsPositive withEpsilon(double newEpsilon) const {
                        return IsPositive{newEpsilon};
                    }
                };
                inline constexpr IsPositive isPositive{defaultEpsilon};

                struct IsNegative {
                    double epsilon;
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

            struct Add {
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

            struct Subtract {
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

            struct Multiply {
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

            struct Divide {
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

            struct Modulo {
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

            struct Equal {
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

            struct NotEqual {
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

            struct LessThan {
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

            struct GreaterThan {
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

            struct LessThanOrEqual {
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

            struct GreaterThanOrEqual {
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

            struct BitwiseAnd {
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

            struct BitwiseOr {
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

            struct BitwiseXor {
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

            struct Minimum {
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left < right ? left : right;
                }
            };
            inline constexpr Minimum minimum{};

            struct Maximum {
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return left > right ? left : right;
                }
            };
            inline constexpr Maximum maximum{};

            struct GreatestCommonDivisor {
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return std::gcd(left, right);
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return std::gcd(left, right);
                    };
                }
            };
            inline constexpr GreatestCommonDivisor greatestCommonDivisor{};

            struct LeastCommonMultiple {
                template<typename LeftType, typename RightType>
                constexpr auto operator()(const LeftType& left, const RightType& right) const {
                    return std::lcm(left, right);
                }

                template<typename RightType>
                constexpr auto operator()(const RightType& right) const {
                    return [right](const auto& left) {
                        return std::lcm(left, right);
                    };
                }
            };
            inline constexpr LeastCommonMultiple leastCommonMultiple{};

            struct IsEven {
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value % 2 == 0;
                }
            };
            inline constexpr IsEven isEven{};

            struct IsOdd {
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value % 2 != 0;
                }
            };
            inline constexpr IsOdd isOdd{};

            struct IsPositive {
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value > 0;
                }
            };
            inline constexpr IsPositive isPositive{};

            struct IsNegative {
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value < 0;
                }
            };
            inline constexpr IsNegative isNegative{};

            struct IsZero {
                template<typename Type>
                constexpr bool operator()(const Type& value) const {
                    return value == 0;
                }
            };
            inline constexpr IsZero isZero{};

        }
    }
}
