#pragma once

#include "../core/fastVector.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace mathematics {

        class Complex {
          public:
          private:
            double real;
            double imaginary;

          public:
            constexpr Complex() : real(0.0), imaginary(0.0) {}

            constexpr Complex(double realValue, double imaginaryValue)
                : real(realValue), imaginary(imaginaryValue) {}

            [[nodiscard]] inline Complex operator+(const Complex& other) const {
                return Complex(real + other.real, imaginary + other.imaginary);
            }

            [[nodiscard]] inline Complex operator-(const Complex& other) const {
                return Complex(real - other.real, imaginary - other.imaginary);
            }

            [[nodiscard]] inline Complex operator*(const Complex& other) const {
                return Complex(real * other.real - imaginary * other.imaginary,
                               real * other.imaginary + imaginary * other.real);
            }

            inline Complex& operator/=(const double scalar) {
                real /= scalar;
                imaginary /= scalar;
                return *this;
            }

            [[nodiscard]] double getReal() const {
                return real;
            }

            [[nodiscard]] double getImaginary() const {
                return imaginary;
            }
        };

        inline void fastFourierTransform(core::FastVector<Complex>& sequence, bool invert) {
            size_t sizeValue = sequence.size();
            assert((sizeValue & (sizeValue - 1)) == 0 && "Size must be a power of 2.");

            for (size_t index = 1, j = 0; index < sizeValue; ++index) {
                size_t bit = sizeValue >> 1;
                for (; j & bit; bit >>= 1) {
                    j ^= bit;
                }
                j ^= bit;
                if (index < j) {
                    std::swap(sequence[index], sequence[j]);
                }
            }

            double piValue = std::acos(-1.0);

            for (size_t length = 2; length <= sizeValue; length <<= 1) {
                size_t halfLength = length >> 1;
                double invertV = invert ? -1.0 : 1.0;
                double angle = 2.0 * piValue / static_cast<double>(length) * invertV;
                Complex root(std::cos(angle), std::sin(angle));

                for (size_t index = 0; index < sizeValue; index += length) {
                    Complex current(1.0, 0.0);
                    for (size_t j = 0; j < halfLength; ++j) {
                        Complex u = sequence[index + j];
                        Complex v = sequence[index + j + halfLength] * current;
                        sequence[index + j] = u + v;
                        sequence[index + j + halfLength] = u - v;
                        current = current * root;
                    }
                }
            }

            if (invert) {
                for (size_t index = 0; index < sizeValue; ++index) {
                    sequence[index] /= static_cast<double>(sizeValue);
                }
            }
        }

        template <typename Type>
        core::FastVector<Type> multiply(const core::FastVector<Type>& first,
                                        const core::FastVector<Type>& second) {
            if (first.empty() || second.empty()) {
                return {};
            }

            size_t resultSize = first.size() + second.size() - 1;
            size_t sizeValue = 1;

            while (sizeValue < resultSize) {
                sizeValue <<= 1;
            }

            core::FastVector<Complex> firstComplex;
            firstComplex.reserve(sizeValue);
            for (size_t index = 0; index < first.size(); ++index) {
                firstComplex.pushBack(Complex(static_cast<double>(first[index]), 0.0));
            }

            for (size_t index = first.size(); index < sizeValue; ++index) {
                firstComplex.pushBack(Complex(0.0, 0.0));
            }

            core::FastVector<Complex> secondComplex;
            secondComplex.reserve(sizeValue);
            for (size_t index = 0; index < second.size(); ++index) {
                secondComplex.pushBack(Complex(static_cast<double>(second[index]), 0.0));
            }
            for (size_t index = second.size(); index < sizeValue; ++index) {
                secondComplex.pushBack(Complex(0.0, 0.0));
            }

            fastFourierTransform(firstComplex, false);
            fastFourierTransform(secondComplex, false);

            for (size_t index = 0; index < sizeValue; ++index) {
                firstComplex[index] = firstComplex[index] * secondComplex[index];
            }

            fastFourierTransform(firstComplex, true);

            core::FastVector<Type> result;
            result.reserve(resultSize);
            for (size_t index = 0; index < resultSize; ++index) {
                Type rounded;
                if (std::is_integral_v<Type>) {
                    rounded = static_cast<Type>(std::round(firstComplex[index].getReal()));
                } else {
                    rounded = static_cast<Type>(firstComplex[index].getReal());
                }
                result.pushBack(rounded);
            }
            return result;
        }

    } // namespace mathematics
} // namespace maistrie
