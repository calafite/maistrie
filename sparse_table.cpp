#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct LinearSieve {
  static_assert(
      is_integral_v<T> && is_signed_v<T> && sizeof(T) >= 4,
      "T must be a signed integer of at least 32 bits (int, long, long long)");

  int n;
  vector<int> primes;
  vector<int> spf;

  LinearSieve(int n) : n(n), spf(n + 1, 0) {
    if (n >= 2) {
      primes.reserve(n / log(n) * 1.2 + 16);
    }
    for (int i = 2; i <= n; i++) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (int p : primes) {
        if (p > spf[i] || 1LL * i * p > n)
          break;
        spf[i * p] = p;
      }
    }
  }

  bool is_prime(T x) const {
    if (x < 2 || x > n)
      return false;
    return spf[x] == x;
  }

  T count(T x) const {
    x = min(static_cast<T>(n), x);
    if (x < 2)
      return 0;
    return upper_bound(primes.begin(), primes.end(), static_cast<int>(x)) -
           primes.begin();
  }

  vector<pair<T, int>> factorize(T x) const {
    assert(x >= 1 && x <= n);
    vector<pair<T, int>> factors;
    while (x > 1) {
      T p = spf[x];
      int e = 0;
      do {
        e++;
        x /= p;
      } while (x > 1 && spf[x] == p);
      factors.push_back({p, e});
    }
    return factors;
  }
};
