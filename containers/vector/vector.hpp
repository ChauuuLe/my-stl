#pragma once

#include "../../memory/uninitialized_memory.hpp"
#include "../../ultilities/equal.hpp"
#include "../../ultilities/lexicographical_compare.hpp"
#include "../../iterators/iterator_traits.hpp"
#include "../../iterators/reverse_iterator.hpp"
#include "../../type_traits/enable_if.hpp"
#include "../../type_traits/is_integral.hpp"
#include "vector_iterator.hpp"
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <memory>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace mystd {
    template<class T, class Allocator = std::allocator<T>>
    class vector: private Allocator {
    private:
        size_t nelem;
        size_t cap;
        T *elems;
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
        using reverse_iterator = mystd::reverse_iterator<iterator>;
        using const_reverse_iterator = mystd::reverse_iterator<const_iterator>;

        /*Iterators*/
        iterator begin();
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator crbegin() const;  
        reverse_iterator rend();
        const_reverse_iterator crend() const;

        /*Capacity*/
        bool empty() const;
        size_type size() const;
        size_type max_size() const;
        void reserve(size_type new_cap);
        size_type capacity() const;
        void shrink_to_fit();

    private:
        size_type get_next_cap() {
            return std::max(size() + 1, capacity() * 2);
        }
    public:
        /*Member functions*/
        allocator_type get_allocator() const;

        void assign(size_type count, const value_type& value);

        /*Ctors*/
        vector();
        explicit vector(const Allocator& alloc);
        explicit vector(size_type count, const Allocator& alloc = Allocator());
        vector(size_type count, const value_type& value, const Allocator& alloc = Allocator());
        template<class InputIt, class = mystd::enable_if_t<!is_integral<InputIt>::value>>
        vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
        vector(const vector& other);
        vector(vector&& other);
        vector(std::initializer_list<value_type> init, const Allocator& alloc = Allocator());

        // Destructor
        ~vector() noexcept(std::is_nothrow_destructible_v<value_type>);

        /*Element access*/
        reference at(size_type pos);
        const_reference at(size_type pos) const;

        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        pointer data();
        const_pointer data() const;

        /*Modifiers*/
        void clear() noexcept(std::is_nothrow_destructible_v<value_type>);

        // iterator insert(const_iterator pos, const value_type& value);
        // iterator insert(const_iterator pos, T&& value);
        // iterator insert(const_iterator pos, size_type count, const T& value);
        // template< class InputIt >
        // iterator insert(const_iterator pos, InputIt first, InputIt last);
        // iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        void push_back(const T& value);
        void push_back(T&& value);

        template<class... Args>
        void emplace_back(Args&&... args);

        void pop_back();

        void resize(size_type count);
        void resize(size_type count, const value_type& value);

        void swap(vector& other) 
                noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                        std::allocator_traits<Allocator>::is_always_equal::value);
    };
}
#include "vector.tpp"
