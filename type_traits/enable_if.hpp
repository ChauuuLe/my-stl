#pragma once

namespace mystd {
    template<bool Cond, T = void>
    struct enable_if {};

    template<true, T>
    struct enable_if {
        using type = T;
    };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B,T>::type;
}