#pragma once

#include "../../ultilities/equal.hpp"
#include "../../ultilities/lexigraphical_compare.hpp"
#include "../../iterators/iterator_traits.hpp"
#include "../../iterators/reverse_iterator.hpp"
#include "../../type_traits/enable_if.hpp"
#include "../../type_traits/is_integral.hpp"
#include<csstdef>
#include<initializer_list>
#include<algorithm>
#include<utility>

template<class T>
class vector {
private:
    T x;
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

}
