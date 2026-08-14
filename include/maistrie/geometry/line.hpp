#pragma once

#include "../io/io.hpp"
#include "point.hpp"
#include <cmath>

namespace maistrie {
    namespace geometry {
        template <typename Type> class Line {
          public:
          public:
            Type a;
            Type b;
            Type c;

            constexpr Line() noexcept : a(Type(0)), b(Type(0)), c(Type(0)) {}

            constexpr Line(Type aValue, Type bValue, Type cValue) noexcept
                : a(aValue), b(bValue), c(cValue) {}

            constexpr Line(Point<Type> first, Point<Type> second) noexcept {
                a = first.y - second.y;
                b = second.x - first.x;
                c = -(a * first.x + b * first.y);
            }

            [[nodiscard]] static constexpr Line
            fromPointAndDirection(Point<Type> point, Point<Type> direction) noexcept {
                return Line(point, point + direction);
            }

            [[nodiscard]] constexpr Type evaluate(Point<Type> point) const noexcept {
                return a * point.x + b * point.y + c;
            }

            [[nodiscard]] constexpr bool contains(Point<Type> point) const noexcept {
                return evaluate(point) == Type(0);
            }

            [[nodiscard]] constexpr bool isParallelTo(const Line& other) const noexcept {
                return a * other.b == b * other.a;
            }

            [[nodiscard]] constexpr bool isOrthogonalTo(const Line& other) const noexcept {
                return a * other.a + b * other.b == Type(0);
            }

            [[nodiscard]] inline double distanceTo(Point<Type> point) const noexcept {
                double num = static_cast<double>(evaluate(point));
                double da = static_cast<double>(a);
                double db = static_cast<double>(b);
                return std::abs(num) / std::sqrt(da * da + db * db);
            }

            [[nodiscard]] inline Point<double>
            intersection(const Line& other, bool* isValid = nullptr) const noexcept {
                double da = static_cast<double>(a);
                double db = static_cast<double>(b);
                double dc = static_cast<double>(c);
                double oa = static_cast<double>(other.a);
                double ob = static_cast<double>(other.b);
                double oc = static_cast<double>(other.c);

                double det = da * ob - db * oa;

                if (det == 0.0) {
                    if (isValid) {
                        *isValid = false;
                    }
                    return Point<double>(0.0, 0.0);
                }

                if (isValid) {
                    *isValid = true;
                }
                double invDet = 1.0 / det;

                return Point<double>((db * oc - dc * ob) * invDet, (dc * oa - da * oc) * invDet);
            }
        };

        template <typename Type>
        inline io::Printer& operator<<(io::Printer& printer, const Line<Type>& line) {
            return printer << line.a << ' ' << line.b << ' ' << line.c;
        }

        template <typename Type>
        inline io::Scanner& operator>>(io::Scanner& scanner, Line<Type>& line) {
            return scanner >> line.a >> line.b >> line.c;
        }
    } // namespace geometry
} // namespace maistrie
