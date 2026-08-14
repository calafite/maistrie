#pragma once

#include "../io/io.hpp"

#include <cmath>

namespace maistrie {
    namespace geometry {
        template <typename Type> class Point {
          public:
          public:
            Type x;
            Type y;

            constexpr Point() : x(Type(0)), y(Type(0)) {}

            constexpr Point(Type xValue, Type yValue) noexcept : x(xValue), y(yValue) {}

            [[nodiscard]] constexpr Point operator+(Point other) const noexcept {
                return Point(x + other.x, y + other.y);
            }

            [[nodiscard]] constexpr Point operator-(Point other) const noexcept {
                return Point(x - other.x, y - other.y);
            }

            [[nodiscard]] constexpr Point operator-() const noexcept {
                return Point(-x, -y);
            }

            [[nodiscard]] constexpr Point operator*(Type scalar) const noexcept {
                return Point(x * scalar, y * scalar);
            }

            [[nodiscard]] constexpr Point operator/(Type scalar) const noexcept {
                return Point(x / scalar, y / scalar);
            }

            constexpr Point& operator+=(Point other) noexcept {
                x += other.x;
                y += other.y;
                return *this;
            }

            constexpr Point& operator-=(Point other) noexcept {
                x -= other.x;
                y -= other.y;
                return *this;
            }

            constexpr Point& operator*=(Type scalar) noexcept {
                x *= scalar;
                y *= scalar;
                return *this;
            }

            constexpr Point& operator/=(Type scalar) noexcept {
                x /= scalar;
                y /= scalar;
                return *this;
            }

            [[nodiscard]] constexpr Type dot(Point other) const noexcept {
                return x * other.x + y * other.y;
            }

            [[nodiscard]] constexpr Type cross(Point other) const noexcept {
                return x * other.y - y * other.x;
            }

            [[nodiscard]] constexpr Type squareNorm() const noexcept {
                return x * x + y * y;
            }

            [[nodiscard]] inline double norm() const noexcept {
                double dx = static_cast<double>(x);
                double dy = static_cast<double>(y);
                return std::sqrt(dx * dx + dy * dy);
            }

            [[nodiscard]] constexpr Type squaredDistance(Point other) const noexcept {
                Type dx = x - other.x;
                Type dy = y - other.y;
                return dx * dx + dy * dy;
            }

            [[nodiscard]] inline double distance(Point other) const noexcept {
                double dx = static_cast<double>(x - other.x);
                double dy = static_cast<double>(y - other.y);
                return std::sqrt(dx * dx + dy * dy);
            }

            [[nodiscard]] constexpr bool operator==(Point other) const noexcept {
                return x == other.x && y == other.y;
            }

            [[nodiscard]] constexpr bool operator!=(Point other) const noexcept {
                return x != other.x || y != other.y;
            }
        };

        template <typename Type>
        [[nodiscard]] constexpr Point<Type> operator*(Type scalar, Point<Type> point) noexcept {
            return point * scalar;
        }

        template <typename Type>
        inline io::Printer& operator<<(io::Printer& printer, Point<Type> point) {
            return printer << point.x << ' ' << point.y;
        }

        template <typename Type>
        inline io::Scanner& operator>>(io::Scanner& scanner, Point<Type>& point) {
            return scanner >> point.x >> point.y;
        }

    } // namespace geometry
} // namespace maistrie
