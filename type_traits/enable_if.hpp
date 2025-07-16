#pragma once

namespace mystd {
    template<bool Cond, T = void>
    struct enable_if {};

    template<true, T>
    struct enable_if {
        using type = T;
    };
}