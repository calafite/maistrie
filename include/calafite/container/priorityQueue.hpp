#pragma once

#include "../core/fastVector.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <initializer_list>
#include <utility>

namespace calafite {
    namespace container {

        template<typename Type, typename Compare = std::less<Type>>
        struct PriorityQueue {
            core::FastVector<Type> data;
            Compare compare;

            PriorityQueue() = default;

            explicit PriorityQueue(Compare comp) : compare(std::move(comp)) {}

            PriorityQueue(std::initializer_list<Type> init, Compare comp = Compare()) 
                : data(init), compare(std::move(comp)) {
                std::make_heap(data.begin(), data.end(), compare);
            }

            template<typename Container>
            PriorityQueue(const Container& container, Compare comp = Compare()) 
                : compare(std::move(comp)) {
                data.reserve(container.size());
                for (const auto& item : container) {
                    data.pushBack(item);
                }
                std::make_heap(data.begin(), data.end(), compare);
            }

            inline bool empty() const { return data.empty(); }
            inline size_t size() const { return data.size(); }

            inline void reserve(size_t capacity) {
                data.reserve(capacity);
            }

            inline const Type& top() const {
                assert(!empty());
                return data.front();
            }

            inline void push(const Type& value) {
                data.pushBack(value);
                std::push_heap(data.begin(), data.end(), compare);
            }

            inline void push(Type&& value) {
                data.pushBack(std::move(value));
                std::push_heap(data.begin(), data.end(), compare);
            }

            template<typename... Args>
            inline void emplace(Args&&... args) {
                data.emplaceBack(std::forward<Args>(args)...);
                std::push_heap(data.begin(), data.end(), compare);
            }

            inline void pop() {
                assert(!empty());
                std::pop_heap(data.begin(), data.end(), compare);
                data.popBack();
            }

            inline Type popValue() {
                assert(!empty());
                std::pop_heap(data.begin(), data.end(), compare);
                Type value = std::move(data.back());
                data.popBack();
                return value;
            }

            inline void clear() {
                data.clear();
            }
        };

        template<typename Type>
        using MinHeap = PriorityQueue<Type, std::greater<Type>>;

        template<typename Type>
        using MaxHeap = PriorityQueue<Type, std::less<Type>>;

    }
}
