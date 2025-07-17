#pragma once

#include "../../ultilities/equal.hpp"
#include "../../ultilities/lexigraphical_compare.hpp"
#include "../../iterators/iterator_traits.hpp"
#include "../../iterators/reverse_iterator.hpp"
#include "../../type_traits/enable_if.hpp"
#include "../../type_traits/is_integral.hpp"
#include "vector_iterator.hpp"
#include <csstdef>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <memory>

namespace mystd {
    template<class T, class Allocator = std::allocator<T>>
    class vector {
    private:
        size_t nelem;
        size_t cap;
        T *elems;
        Allocator allocator;
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = vector_iterator<T>;
        using const_iterator = vector_iterator<const T>;
        using reverse_iterator = reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator<const_iterator>;
        
        /*Ctors*/
        vector();
        explicit vector(const Allocator& alloc);
        explicit vector(size_type count, const Allocator& alloc = Allocator());
        vector(size_type count, const T& value, const Allocator& alloc = Allocator());
        template<class InputIt>
        vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
        vector(const vector& other);
        vector(vector&& other);
        vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());
    };
}
