#pragma once

namespace mystd {
    template<class T = void>
    struct less;

    template<class T>
    struct less {
        constexpr bool operator()(const T& lhs, const T& rhs) const {
            return lhs < rhs;
        }
    };

    template<>
    struct less<void> {
        template<class U, class V>
        constexpr auto operator()(U&& lhs, V&& rhs) const
            -> decltype(std::forward<U>(lhs) < std::forward<V>(rhs)) {
            return std::forward<U>(lhs) < std::forward<V>(rhs);
        }
    };
}
