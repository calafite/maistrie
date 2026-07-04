#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace calafite {
    namespace arena {
        extern char* pointer;
        extern char* end;
        extern bool active;
    } // namespace arena
} // namespace calafite

#if defined(__GNUC__) || defined(__clang__)
#define CALAFITE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CALAFITE_UNLIKELY(x) (x)
#endif

namespace calafite {
    namespace core {

        template <typename Type> class FastVector {
          public:
            using iterator = Type*;
            using const_iterator = const Type*;
            using value_type = Type;

            FastVector() = default;

            explicit FastVector(size_t initialSize) {
                if (initialSize > 0) {
                    pointer = static_cast<Type*>(::operator new[](initialSize * sizeof(Type)));
                    sizeValue = initialSize;
                    capacityValue = initialSize;
                }
            }

            FastVector(const Type* arrayPointer, size_t count) {
                if (count > 0) {
                    pointer = static_cast<Type*>(::operator new[](count * sizeof(Type)));
                    sizeValue = count;
                    capacityValue = count;
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::memcpy(pointer, arrayPointer, count * sizeof(Type));
                    } else {
                        for (size_t i = 0; i < count; ++i) {
                            new (&pointer[i]) Type(arrayPointer[i]);
                        }
                    }
                }
            }

            FastVector(size_t initialSize, const Type& initialElement) {
                if (initialSize > 0) {
                    pointer = static_cast<Type*>(::operator new[](initialSize * sizeof(Type)));
                    sizeValue = initialSize;
                    capacityValue = initialSize;
                }
                if constexpr (std::is_trivially_copyable_v<Type>) {
                    std::fill_n(pointer, initialSize, initialElement);
                } else {
                    for (size_t i = 0; i < initialSize; i++) {
                        new (pointer + i) Type(initialElement);
                    }
                }
            }

            template <size_t N> explicit FastVector(const Type (&arr)[N]) {
                pointer = static_cast<Type*>(::operator new[](N * sizeof(Type)));
                sizeValue = N;
                capacityValue = N;
                if constexpr (std::is_trivially_copyable_v<Type>) {
                    std::memcpy(pointer, arr, N * sizeof(Type));
                } else {
                    for (size_t i = 0; i < N; ++i) {
                        new (&pointer[i]) Type(arr[i]);
                    }
                }
            }

            template <size_t N> explicit FastVector(const std::array<Type, N>& arr) {
                pointer = static_cast<Type*>(::operator new[](N * sizeof(Type)));
                sizeValue = N;
                capacityValue = N;
                if constexpr (std::is_trivially_copyable_v<Type>) {
                    std::memcpy(pointer, arr.data(), N * sizeof(Type));
                } else {
                    for (size_t i = 0; i < N; ++i) {
                        new (&pointer[i]) Type(arr[i]);
                    }
                }
            }

            FastVector(std::initializer_list<Type> init) {
                size_t count = init.size();
                if (count > 0) {
                    pointer = static_cast<Type*>(::operator new[](count * sizeof(Type)));
                    sizeValue = count;
                    capacityValue = count;
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::memcpy(pointer, init.begin(), count * sizeof(Type));
                    } else {
                        size_t i = 0;
                        for (const auto& item : init) {
                            new (&pointer[i++]) Type(item);
                        }
                    }
                }
            }

            ~FastVector() {
                if (pointer) {
                    if constexpr (!std::is_trivially_destructible_v<Type>) {
                        for (size_t i = 0; i < sizeValue; ++i) {
                            pointer[i].~Type();
                        }
                    }
                    ::operator delete[](pointer);
                }
            }

            FastVector(const FastVector& other)
                : pointer(nullptr), sizeValue(other.sizeValue), capacityValue(other.sizeValue) {
                if (capacityValue > 0) {
                    pointer = static_cast<Type*>(::operator new[](capacityValue * sizeof(Type)));
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::memcpy(pointer, other.pointer, sizeValue * sizeof(Type));
                    } else {
                        for (size_t i = 0; i < sizeValue; ++i) {
                            new (&pointer[i]) Type(other.pointer[i]);
                        }
                    }
                }
            }

            FastVector& operator=(const FastVector& other) {
                if (this != &other) {
                    clear();
                    if (other.sizeValue > capacityValue) {
                        if (pointer)
                            ::operator delete[](pointer);
                        capacityValue = other.sizeValue;
                        pointer = capacityValue > 0
                            ? static_cast<Type*>(::operator new[](capacityValue * sizeof(Type)))
                            : nullptr;
                    }
                    sizeValue = other.sizeValue;
                    if (sizeValue > 0) {
                        if constexpr (std::is_trivially_copyable_v<Type>) {
                            std::memcpy(pointer, other.pointer, sizeValue * sizeof(Type));
                        } else {
                            for (size_t i = 0; i < sizeValue; ++i) {
                                new (&pointer[i]) Type(other.pointer[i]);
                            }
                        }
                    }
                }
                return *this;
            }

            FastVector(FastVector&& other) noexcept
                : pointer(other.pointer), sizeValue(other.sizeValue),
                  capacityValue(other.capacityValue) {
                other.pointer = nullptr;
                other.sizeValue = 0;
                other.capacityValue = 0;
            }

            FastVector& operator=(FastVector&& other) noexcept {
                if (this != &other) {
                    this->~FastVector();
                    pointer = other.pointer;
                    sizeValue = other.sizeValue;
                    capacityValue = other.capacityValue;
                    other.pointer = nullptr;
                    other.sizeValue = 0;
                    other.capacityValue = 0;
                }
                return *this;
            }

            FastVector& operator=(std::initializer_list<Type> init) {
                size_t count = init.size();
                clear();
                if (count > capacityValue)
                    grow(count);
                if (count > 0) {
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::memcpy(pointer, init.begin(), count * sizeof(Type));
                    } else {
                        size_t i = 0;
                        for (const auto& item : init) {
                            new (&pointer[i++]) Type(item);
                        }
                    }
                }
                sizeValue = count;
                return *this;
            }

            inline void pushBack(const Type& value) {
                if (CALAFITE_UNLIKELY(sizeValue == capacityValue))
                    grow();
                new (&pointer[sizeValue++]) Type(value);
            }

            inline void pushBack(Type&& value) {
                if (CALAFITE_UNLIKELY(sizeValue == capacityValue))
                    grow();
                new (&pointer[sizeValue++]) Type(std::move(value));
            }

            template <typename... Args> inline Type& emplaceBack(Args&&... args) {
                if (CALAFITE_UNLIKELY(sizeValue == capacityValue))
                    grow();
                new (&pointer[sizeValue]) Type(std::forward<Args>(args)...);
                return pointer[sizeValue++];
            }

            inline void popBack() {
                assert(sizeValue > 0);
                --sizeValue;
                if constexpr (!std::is_trivially_destructible_v<Type>) {
                    pointer[sizeValue].~Type();
                }
            }

            inline void reserve(size_t newCapacity) {
                if (newCapacity > capacityValue)
                    grow(newCapacity);
            }

            inline void resize(size_t newSize) {
                if (newSize > capacityValue)
                    grow(newSize);

                if (newSize > sizeValue) {
                    if constexpr (!std::is_trivial_v<Type>) {
                        for (size_t i = sizeValue; i < newSize; ++i) {
                            new (&pointer[i]) Type();
                        }
                    }
                } else {
                    if constexpr (!std::is_trivially_destructible_v<Type>) {
                        for (size_t i = newSize; i < sizeValue; ++i) {
                            pointer[i].~Type();
                        }
                    }
                }
                sizeValue = newSize;
            }

            inline void assign(size_t count, const Type& value) {
                Type copy = value;
                clear();
                if (count > capacityValue)
                    grow(count);
                if (count > 0) {
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::fill_n(pointer, count, copy);
                    } else {
                        for (size_t i = 0; i < count; ++i) {
                            new (&pointer[i]) Type(copy);
                        }
                    }
                }
                sizeValue = count;
            }

            inline void clear() {
                if constexpr (!std::is_trivially_destructible_v<Type>) {
                    for (size_t i = 0; i < sizeValue; ++i) {
                        pointer[i].~Type();
                    }
                }
                sizeValue = 0;
            }

            inline void reverse() {
                if (sizeValue > 1) {
                    for (size_t i = 0, j = sizeValue - 1; i < j; ++i, --j) {
                        std::swap(pointer[i], pointer[j]);
                    }
                }
            }

            inline Type& operator[](size_t index) {
                assert(index < sizeValue);
                return pointer[index];
            }

            inline const Type& operator[](size_t index) const {
                assert(index < sizeValue);
                return pointer[index];
            }

            inline Type& front() {
                assert(sizeValue > 0);
                return pointer[0];
            }

            inline const Type& front() const {
                assert(sizeValue > 0);
                return pointer[0];
            }

            inline Type& back() {
                assert(sizeValue > 0);
                return pointer[sizeValue - 1];
            }

            inline const Type& back() const {
                assert(sizeValue > 0);
                return pointer[sizeValue - 1];
            }

            inline size_t size() const {
                return sizeValue;
            }

            inline size_t capacity() const {
                return capacityValue;
            }

            inline bool empty() const {
                return sizeValue == 0;
            }

            inline Type* data() {
                return pointer;
            }

            inline const Type* data() const {
                return pointer;
            }

            inline iterator begin() {
                return iterator(pointer);
            }

            inline iterator end() {
                return iterator(pointer + sizeValue);
            }

            inline const_iterator begin() const {
                return const_iterator(pointer);
            }

            inline const_iterator end() const {
                return const_iterator(pointer + sizeValue);
            }

            template <typename Compare> inline void sort(Compare comp) {
                if (sizeValue > 1)
                    std::sort(begin(), end(), comp);
            }

            inline void sort() {
                if (sizeValue > 1)
                    std::sort(begin(), end());
            }

            inline void unique() {
                if (sizeValue > 1) {
                    auto it = std::unique(begin(), end());
                    size_t newSize = static_cast<size_t>(it - begin());
                    if constexpr (!std::is_trivially_destructible_v<Type>) {
                        for (size_t i = newSize; i < sizeValue; ++i) {
                            pointer[i].~Type();
                        }
                    }
                    sizeValue = newSize;
                }
            }

          private:
            Type* pointer = nullptr;
            size_t sizeValue = 0;
            size_t capacityValue = 0;

            [[gnu::noinline]] void grow(size_t minimumCapacity = 0) {
                size_t newCapacity = capacityValue == 0 ? 4 : capacityValue * 2;
                if (newCapacity < minimumCapacity)
                    newCapacity = minimumCapacity;

                if (calafite::arena::active && pointer) {
                    char* allocationTail =
                        reinterpret_cast<char*>(pointer) + (capacityValue * sizeof(Type));
                    if (allocationTail == calafite::arena::pointer) {
                        size_t bytesNeeded = (newCapacity - capacityValue) * sizeof(Type);
                        if (calafite::arena::pointer + bytesNeeded <= calafite::arena::end) {
                            calafite::arena::pointer += bytesNeeded;
                            capacityValue = newCapacity;
                            return;
                        }
                    }
                }

                Type* newPointer = static_cast<Type*>(::operator new[](newCapacity * sizeof(Type)));

                if (pointer) {
                    if constexpr (std::is_trivially_copyable_v<Type>) {
                        std::memcpy(newPointer, pointer, sizeValue * sizeof(Type));
                    } else {
                        for (size_t i = 0; i < sizeValue; ++i) {
                            new (&newPointer[i]) Type(std::move(pointer[i]));
                            pointer[i].~Type();
                        }
                    }
                    ::operator delete[](pointer);
                }

                pointer = newPointer;
                capacityValue = newCapacity;
            }
        };

    } // namespace core
} // namespace calafite
