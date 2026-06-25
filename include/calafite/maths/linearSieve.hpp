#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace calafite {
    namespace mathematics {

        template<typename Type> struct LinearSieve {
            static_assert(
                std::is_integral_v<Type> && std::is_signed_v<Type> && sizeof(Type) >= 4,
                "Type must be a signed integer of at least 32 bits (int, long, long long)");

            Type limit;
            core::FastVector<Type> primes;
            core::FastVector<Type> smallestPrimeFactor;

            LinearSieve(Type limit) : limit(limit), smallestPrimeFactor((limit >> 1) + 1, 0) {
                if (limit >= 2) {
                    primes.reserve(static_cast<size_t>(limit / std::log(static_cast<double>(limit)) * 1.2) + 16);
                    primes.pushBack(2);
                }
                for (Type index = 3; index <= this->limit; index += 2) {
                    Type halfIndex = index >> 1;
                    if (!smallestPrimeFactor[halfIndex]) {
                        smallestPrimeFactor[halfIndex] = index;
                        primes.pushBack(index);
                    }

                    for (size_t primeIndex = 1; primeIndex < primes.size(); ++primeIndex) {
                        Type prime = primes[primeIndex];
                        if (static_cast<long long>(prime) * index > this->limit) break;
                        smallestPrimeFactor[(index * prime) >> 1] = prime;
                        if (prime == smallestPrimeFactor[halfIndex]) break;
                    }
                }
            }

            bool query(Type number) const {
                if (number < 2 || number > limit) return false;
                if (number == 2) return true;
                if ((number & 1) == 0) return false;
                return smallestPrimeFactor[number >> 1] == number;
            }

            Type count(Type number) const {
                number = std::min(number, limit);
                if (number < 2) return 0;
                return static_cast<Type>(std::upper_bound(primes.begin(), primes.end(), number) - primes.begin());
            }

            core::FastVector<std::pair<Type, int>> factorize(Type number) const {
                assert(number >= 1 && number <= limit);
                core::FastVector<std::pair<Type, int>> factors;

                if ((number & 1) == 0) {
                    int exponent = 0;
                    do {
                        exponent++;
                        number >>= 1;
                    } while ((number & 1) == 0);
                    factors.emplaceBack(2, exponent);
                }

                while (number > 1) {
                    Type prime = smallestPrimeFactor[number >> 1];
                    int exponent = 0;
                    do {
                        exponent++;
                        number /= prime;
                    } while (number > 1 && smallestPrimeFactor[number >> 1] == prime);
                    factors.emplaceBack(prime, exponent);
                }
                
                return factors;
            }
        };

    }
}
