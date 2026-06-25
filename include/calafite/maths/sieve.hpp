#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace calafite {
    namespace mathematics {

        template<typename Type = int> struct Sieve {
            static_assert(std::is_integral_v<Type> && std::is_signed_v<Type> && sizeof(Type) >= 4,
                          "Type must be a signed integer of at least 32 bits");

            Type limit;
            core::FastVector<bool> isPrime;
            core::FastVector<int> primes;

            Sieve(Type limit) : limit(limit), isPrime((limit >> 1) + 1, true) {
                if (isPrime.size() > 0) isPrime[0] = false;

                for (Type prime = 3; static_cast<long long>(prime) * prime <= limit; prime += 2) {
                    if (isPrime[static_cast<size_t>(prime >> 1)]) {
                        for (long long multiple = static_cast<long long>(prime) * prime; multiple <= limit; multiple += prime * 2) {
                            isPrime[static_cast<size_t>(multiple >> 1)] = false;
                        }
                    }
                }

                if (limit >= 2) {
                    primes.reserve(static_cast<size_t>(limit / std::log(limit) * 1.2) + 16);
                    primes.pushBack(2);
                    for (Type number = 3; number <= limit; number += 2) {
                        if (isPrime[static_cast<size_t>(number >> 1)]) primes.pushBack(static_cast<int>(number));
                    }
                }
            }

            bool query(Type number) const {
                if (number < 2 || number > limit) return false;
                if (number == 2) return true;
                if ((number & 1) == 0) return false;
                return isPrime[static_cast<size_t>(number >> 1)];
            }

            Type count(Type number) const {
                number = std::min(static_cast<Type>(limit), number);
                if (number < 2) return 0;
                return static_cast<Type>(std::upper_bound(primes.begin(), primes.end(), static_cast<int>(number)) - primes.begin());
            }
        };

    }
}
