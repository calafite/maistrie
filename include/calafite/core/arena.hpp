#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace calafite {
    namespace arena {

        struct Chunk {
            Chunk* next;
            size_t size;
            char* data() { return reinterpret_cast<char*>(this + 1); }
        };

        inline Chunk* head = nullptr;
        inline Chunk* current = nullptr;
        inline char* pointer = nullptr;
        inline char* end = nullptr;
        inline bool active = false;

        inline void addChunk(size_t minimum) {
            size_t size = 1024 * 1024;
            if (current) size = current->size * 2;
            if (size < minimum) size = minimum;

            Chunk* chunk = static_cast<Chunk*>(std::malloc(sizeof(Chunk) + size));
            chunk->next = nullptr;
            chunk->size = size;

            if (current) current->next = chunk;
            if (!head) head = chunk;
            current = chunk;
        }

        inline void* allocate(size_t size, size_t alignment = 16) noexcept {
            assert(size > 0);
            uintptr_t address = reinterpret_cast<uintptr_t>(pointer);
            uintptr_t aligned = (address + alignment - 1) & ~(alignment - 1);

            if (aligned + size > reinterpret_cast<uintptr_t>(end)) {
                if (current && current->next && current->next->size >= size + alignment) {
                    current = current->next;
                } else {
                    addChunk(size + alignment);
                }
                pointer = current->data();
                end = pointer + current->size;
                aligned = (reinterpret_cast<uintptr_t>(pointer) + alignment - 1) & ~(alignment - 1);
            }

            pointer = reinterpret_cast<char*>(aligned + size);
            return reinterpret_cast<void*>(aligned);
        }

        inline void reset() noexcept {
            if (!head) return;
            current = head;
            pointer = current->data();
            end = pointer + current->size;
        }

        inline bool contains(void* target) noexcept {
            for (Chunk* chunk = head; chunk; chunk = chunk->next) {
                if (target >= chunk->data() && target < chunk->data() + chunk->size) return true;
            }
            return false;
        }

        struct ScopedArena {
            ScopedArena() { active = true; }
            ~ScopedArena() {
                active = false;
                reset();
            }
        };

    }
}

#if defined(_WIN32)
#include <malloc.h>
#define CALAFITE_ALIGNED_ALLOC(alignment, size) _aligned_malloc(size, alignment)
#define CALAFITE_ALIGNED_FREE(pointer) _aligned_free(pointer)
#else
#define CALAFITE_ALIGNED_ALLOC(alignment, size) std::aligned_alloc(alignment, size)
#define CALAFITE_ALIGNED_FREE(pointer) std::free(pointer)
#endif

#define CALAFITE_MAKE_ARENA_GLOBAL                                             \
  void* operator new(size_t size) {                                            \
    if (calafite::arena::active) return calafite::arena::allocate(size);       \
    return std::malloc(size);                                                  \
  }                                                                            \
  void* operator new[](size_t size) {                                          \
    if (calafite::arena::active) return calafite::arena::allocate(size);       \
    return std::malloc(size);                                                  \
  }                                                                            \
  void* operator new(size_t size, std::align_val_t alignmentValue) {           \
    if (calafite::arena::active) return calafite::arena::allocate(size, static_cast<size_t>(alignmentValue)); \
    size_t alignment = static_cast<size_t>(alignmentValue);                    \
    size_t remainder = size & (alignment - 1);                                 \
    return CALAFITE_ALIGNED_ALLOC(alignment, remainder == 0 ? size : size + (alignment - remainder)); \
  }                                                                            \
  void* operator new[](size_t size, std::align_val_t alignmentValue) {         \
    if (calafite::arena::active) return calafite::arena::allocate(size, static_cast<size_t>(alignmentValue)); \
    size_t alignment = static_cast<size_t>(alignmentValue);                    \
    size_t remainder = size % alignment;                                       \
    return CALAFITE_ALIGNED_ALLOC(alignment, remainder == 0 ? size : size + (alignment - remainder)); \
  }                                                                            \
  void operator delete(void* pointer) noexcept {                               \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    std::free(pointer);                                                        \
  }                                                                            \
  void operator delete[](void* pointer) noexcept {                             \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    std::free(pointer);                                                        \
  }                                                                            \
  void operator delete(void* pointer, size_t) noexcept {                       \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    std::free(pointer);                                                        \
  }                                                                            \
  void operator delete[](void* pointer, size_t) noexcept {                     \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    std::free(pointer);                                                        \
  }                                                                            \
  void operator delete(void* pointer, std::align_val_t) noexcept {             \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    CALAFITE_ALIGNED_FREE(pointer);                                            \
  }                                                                            \
  void operator delete[](void* pointer, std::align_val_t) noexcept {           \
    if (!pointer || calafite::arena::active || calafite::arena::contains(pointer)) return; \
    CALAFITE_ALIGNED_FREE(pointer);                                            \
  }
