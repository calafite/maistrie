#pragma once
#include <utility>

namespace maistrie {
    namespace utility {
        template <typename Functor> class FixedPoint {

          private:
            Functor functor;

          public:
            template <typename GeneratorType>
            explicit FixedPoint(GeneratorType&& generator)
                : functor(std::forward<GeneratorType>(generator)) {}

            template <typename... Arguments> decltype(auto) operator()(Arguments&&... arguments) {
                return functor(*this, std::forward<Arguments>(arguments)...);
            }
        };

        template <typename Functor> FixedPoint(Functor) -> FixedPoint<Functor>;
    } // namespace utility
} // namespace maistrie
