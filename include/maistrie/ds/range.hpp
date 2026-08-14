#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace maistrie {
    namespace ds {
        template <typename Container, typename = void> struct IsRange : std::false_type {};

        template <typename Container>
        struct IsRange<Container,
                       std::void_t<decltype(std::begin(std::declval<const Container&>())),
                                   decltype(std::end(std::declval<const Container&>()))>>
            : std::true_type {};
    } // namespace ds
} // namespace maistrie
