#pragma once

#include <chrono>
#include <cstdint>
#include <vector>

namespace calafite {

struct CustomHash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

template <typename K, typename V, typename Hash = CustomHash> struct hash_map {
  int cap;
  std::vector<K> keys;
  std::vector<V> vals;
  std::vector<bool> occupied;
  int sz;
  Hash hasher;

  hash_map(int capacity = 1 << 4) {
    cap = 1;
    while (cap < capacity)
      cap <<= 1;
    keys.resize(cap);
    vals.resize(cap);
    occupied.assign(cap, false);
    sz = 0;
  }

  inline void check_expand() {
    if (sz * 2 >= cap) {
      int old_cap = cap;
      cap <<= 1;
      std::vector<K> old_keys = std::move(keys);
      std::vector<V> old_vals = std::move(vals);
      std::vector<bool> old_occ = std::move(occupied);

      keys.assign(cap, K());
      vals.assign(cap, V());
      occupied.assign(cap, false);
      sz = 0;

      for (int i = 0; i < old_cap; i++) {
        if (old_occ[i])
          insert(old_keys[i], old_vals[i]);
      }
    }
  }

  inline int lookup(K key) const {
    int pos = hasher(key) & (cap - 1);
    while (occupied[pos] && keys[pos] != key) {
      pos = (pos + 1) & (cap - 1);
    }
    return pos;
  }

  V &operator[](K key) {
    check_expand();
    int pos = lookup(key);
    if (!occupied[pos]) {
      occupied[pos] = true;
      keys[pos] = key;
      vals[pos] = V();
      sz++;
    }
    return vals[pos];
  }

  void insert(K key, V val) { (*this)[key] = val; }

  bool count(K key) const {
    int pos = lookup(key);
    return occupied[pos];
  }
};
} // namespace calafite
