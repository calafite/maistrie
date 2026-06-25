#pragma once

#include "../core/fastVector.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <utility>


namespace calafite {
    namespace container {
                
        template<typename Type> struct IsPair : std::false_type {};

        template<typename Type1, typename Type2>
        struct IsPair<std::pair<Type1, Type2>> : std::true_type {};

        template<typename Type> struct CustomHash {
            static uint64_t splitMix64(uint64_t value) {
                value += 0x9e3779b97f4a7c15;
                value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9;
                value = (value ^ (value >> 27)) * 0x94d049bb133111eb;
                return value ^ (value >> 31);
            }

            size_t operator()(const Type& value) const {
                static const uint64_t fixedRandom = std::chrono::steady_clock::now().time_since_epoch().count();

                if constexpr (IsPair<Type>::value) {
                    size_t seed = std::hash<typename Type::first_type>{}(value.first);
                    seed ^= std::hash<typename Type::second_type>{}(value.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                    return splitMix64(seed + fixedRandom);
                } else {
                    return splitMix64(std::hash<Type>{}(value) + fixedRandom);
                }
            }
        };

        template<typename KeyType, typename ValueType, typename HashFunction = CustomHash<KeyType>>
        struct HashMap {
            struct Entry {
                KeyType key;
                ValueType value;
                bool occupied = false;
            };

            size_t capacity;
            core::FastVector<Entry> table;
            size_t size;
            HashFunction hasher;

            HashMap(size_t initialCapacity = 16) : capacity(1), size(0) {
                while (capacity < initialCapacity) capacity <<= 1;
                table.resize(capacity);
            }

            inline void checkExpand() {
                if (size * 2 >= capacity) {
                    size_t oldCapacity = capacity;
                    capacity <<= 1;
                    core::FastVector<Entry> oldTable = std::move(table);

                    table.assign(capacity, Entry());
                    size = 0;

                    for (size_t index = 0; index < oldCapacity; ++index) {
                        if (oldTable[index].occupied) {
                            size_t position = lookup(oldTable[index].key);
                            table[position].occupied = true;
                            table[position].key = std::move(oldTable[index].key);
                            table[position].value = std::move(oldTable[index].value);
                            ++size;
                        }
                    }
                }
            }

            inline size_t lookup(const KeyType& key) const {
                assert(capacity > 0);
                size_t position = hasher(key) & (capacity - 1);
                
                while (table[position].occupied && table[position].key != key) {
                    position = (position + 1) & (capacity - 1);
                }
                
                return position;
            }

            ValueType& operator[](const KeyType& key) {
                checkExpand();
                size_t position = lookup(key);
                
                if (!table[position].occupied) {
                    table[position].occupied = true;
                    table[position].key = key;
                    table[position].value = ValueType();
                    ++size;
                }
                
                return table[position].value;
            }

            void insert(KeyType key, ValueType value) {
                checkExpand();
                size_t position = lookup(key);
                
                if (!table[position].occupied) {
                    table[position].occupied = true;
                    table[position].key = std::move(key);
                    table[position].value = std::move(value);
                    ++size;
                } else {
                    table[position].value = std::move(value);
                }
            }

            bool count(const KeyType& key) const {
                size_t position = lookup(key);
                return table[position].occupied;
            }

            void clear() {
                table.assign(capacity, Entry());
                size = 0;
            }
        };

    }
}
