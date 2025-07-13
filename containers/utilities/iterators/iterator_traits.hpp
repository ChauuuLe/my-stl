#pragma once
#include<cstddef>
#include<iterator>

namespace mystd {
    template<class Iterator>
    class iterator_traits {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    template<class T>
    class iterator_traits<T*> {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename T;
        using difference_type = std::ptrdiff_t;
        using pointer = typename T*;
        using reference = typename T&;
    };

    template<class T>
    class iterator_traits<const T*> {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename T;
        using difference_type = std::ptrdiff_t;
        using pointer = typename const T*;
        using reference = typename const T&;
    };
}