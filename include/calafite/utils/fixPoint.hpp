#pragma once
#include <utility>

namespace calafite {
    namespace utility {

        template<typename Functor> 
        struct FixedPoint {
            Functor functor;

            template<typename GeneratorType> 
            explicit FixedPoint(GeneratorType&& generator) : functor(std::forward<GeneratorType>(generator)) {}

            template<typename... Arguments> 
            decltype(auto) operator()(Arguments&&... arguments) {
                return functor(*this, std::forward<Arguments>(arguments)...);
            }
        };

        template<typename Functor>
        FixedPoint(Functor) -> FixedPoint<Functor>;

    }
}
