#pragma once

#include "../core/fastVector.hpp"
#include "range.hpp"
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace ds {

        template <typename Type, typename Compare = std::less<Type>> class CartesianTree {
          public:
            static constexpr size_t nullNode = static_cast<size_t>(-1);

          private:
            size_t sizeValue;
            core::FastVector<size_t> leftChild;
            core::FastVector<size_t> rightChild;
            core::FastVector<size_t> parent;
            size_t root;
            Compare compare;

          public:
            CartesianTree() : sizeValue(0), root(nullNode) {}

            template <typename Iter>
            CartesianTree(Iter first, Iter last, Compare comp = Compare())
                : sizeValue(static_cast<size_t>(std::distance(first, last))),
                  leftChild(sizeValue, nullNode), rightChild(sizeValue, nullNode),
                  parent(sizeValue, nullNode), root(nullNode), compare(std::move(comp)) {

                if (sizeValue == 0)
                    return;

                core::FastVector<Type> values(sizeValue);
                size_t index = 0;
                for (Iter it = first; it != last; ++it, ++index) {
                    values[index] = *it;
                }

                core::FastVector<size_t> stack;
                stack.reserve(sizeValue);

                for (index = 0; index < sizeValue; ++index) {
                    size_t lastPopped = nullNode;

                    while (!stack.empty() && compare(values[index], values[stack.back()])) {
                        lastPopped = stack.back();
                        stack.popBack();
                    }

                    if (lastPopped != nullNode) {
                        leftChild[index] = lastPopped;
                        parent[lastPopped] = index;
                    }

                    if (!stack.empty()) {
                        rightChild[stack.back()] = index;
                        parent[index] = stack.back();
                    }

                    stack.pushBack(index);
                }

                root = stack.front();
            }

            template <typename Container, typename = std::enable_if_t<IsRange<Container>::value>>
            CartesianTree(const Container& values, Compare comp = Compare())
                : CartesianTree(std::begin(values), std::end(values), std::move(comp)) {}
        };

        template <typename Container>
        CartesianTree(const Container&) -> CartesianTree<typename Container::value_type>;

        template <typename Container, typename Compare>
        CartesianTree(const Container&, Compare)
            -> CartesianTree<typename Container::value_type, Compare>;

        template <typename Iter>
        CartesianTree(Iter, Iter) -> CartesianTree<typename std::iterator_traits<Iter>::value_type>;

        template <typename Iter, typename Compare>
        CartesianTree(Iter, Iter, Compare)
            -> CartesianTree<typename std::iterator_traits<Iter>::value_type, Compare>;

    } // namespace ds
} // namespace maistrie
