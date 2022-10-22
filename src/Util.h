//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_UTIL_H
#define HOVERGAME_UTIL_H

#include "Includes.h"

class Util {
public:
    template <typename T>
    static auto ToVector(T r);
};

template<typename T>
auto Util::ToVector(T r) {
    std::vector<std::ranges::range_value_t<decltype(r)>> v;

    if constexpr(std::ranges::sized_range<decltype(r)>) {
        v.reserve(std::ranges::size(r));
    }

    std::ranges::copy(r, std::back_inserter(v));

    return v;
}


#endif //HOVERGAME_UTIL_H
