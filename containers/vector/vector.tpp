#pragma once
#include "vector.hpp"

namespace mystd {
    /*Capacity*/
    template<class T, class Allocator = std::allocator<T>>
    bool vector<T, Allocator>::empty() const {
        return (this->nelem == true);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const {
        return this->nelem;
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const {
        return this->cap;
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const {
        const size_type allocator_limit = std::allocator_traits<Allocator>::max_size(this->allocator);
        const size_type sys_limit = std::numeric_limits<difference_type>::max();

        return (allocator_limit < sys_limit) ? allocator_limit : sys_limit;
    }

    template<class T, class Allocator = std::allocator<T>>
    void vector<T, Allocator>::shrink_to_fit() {
        try {
            std::allocator_traits<Allocator>::deallocate(this->allocator, this->end(), this->cap - this->nelem);
            this->cap = this->nelem;
        } catch(...) {
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    void vector<T, Allocator>::reserve(size_type new_cap) {
        if (new_cap <= this->cap) {
            return;
        }

        pointer new_placement = std::allocator_traits<Allocator>::allocate(this->alloc, new_cap);

        try {
            uninitialized_copy_construct(this->begin(), this->size(), iterator(new_placement));
            this->cap = new_cap;
            this->elems = new_placement;
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->allocator, new_placement, new_cap);
        }
    }

    /*Iterators*/
    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() const {
        return iterator(this->elems);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const {
        return const_iterator(this->elems);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::end() const {
        return iterator(this->elems + this->nlem * sizeof(T));
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const {
        return const_iterator(this->elems + this->nlem * sizeof(T));
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() const {
        return reverse_iterator(this->end());
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const {
        return const_reverse_iterator(this->end());
    }
    
    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() const {
        return reverse_iterator(this->begin());
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const {
        return const_reverse_iterator(this->begin());
    }
    
    /*Member function*/
    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() {
        return this->allocator;
    }

    /*Ctors*/
    template<class T, class Allocator = std::allocator<T>>
    vector<T, Allocator>::vector(): allocator(Allocator()), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(const Allocator& alloc): allocator(alloc), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(size_type count, const Allocator& alloc = Allocator())
        : allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        this->elems = std::allocator_traits<Allocator>::allocate(this->alloc, count);
        this->cap = count;

        try {
            uninitialized_default_construct(this->begin(), count);
            this->nelem = count;
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->alloc, this->elems, count);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(size_type count, const value_type& value, const Allocator& alloc = Allocator())
        : allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        this->elems = std::allocator_traits<Allocator>::allocate(this->alloc, count);
        this->cap = count;

        try {
            uninitialized_copy_value_construct(value, count, this->begin());
            this->nelem = count;
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->alloc, this->elems, count);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    template<class InputIt>
    vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        this->nelem = std::distance(first, last);
        this->elems = std::allocator_traits<Allocator>::allocate(this->allocator, this->nelem);
        this->cap = this->nelem;

        try {
            uninitialized_copy_construct(first, this->size(), this->begin());
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->allocator, this->elems, this->nelem);
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    vector<T, Allocator>::vector(const vector& other)
        : allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())) {
        this->nelem = other.nelem;
        this->elems = std::allocator_traits<Allocator>::allocate(this->allocator, this->nelem);
        this->cap = this->nelem;

        try {
            uninitialized_copy_construct(other.begin(), this->size(), this->begin());
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->allocator, this->elems, this->nelem);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(vector&& other)
        : allocator(std::move(other.allocator)), nelem(other.nelem),
          cap(other.cap), elems(other.elems) {
        other.elems = nullptr;
        other.nelem = 0;
        other.cap = 0;
    }

    template<class T, class Allocator = std::allocator<T>>
    vector<T, Allocator>::vector(std::initializer_list<value_type> init, const Allocator& alloc = Allocator())
        : allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
          nelem(init.size()), cap(init.size()) {
        this->elems = std::allocator_traits<Allocator>::allocate(this->allocator, this->nelem);

        try {
            uninitialized_copy_construct(init.begin(), this->size(), this->begin());
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->allocator, this->elems, this->nelem);
            throw;
        }
    }

    /*Destructor*/
    template<class T, class Allocator = std::allocator<T>>
    vector<T, Allocator>::~vector() {
        std::destroy(this->begin(), this->end());
        std::allocator_traits<Allocator>::deallocate(this->allocator, this->elems, this->capacity());
    }

    /*Element access*/
    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos) {
        if (pos >= this->size()) {
            throw std::out_of_range("Access out of range for std vector");
        } 
        return *(this->elems + pos);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const {
        if (pos >= this->size()) {
            throw std::out_of_range("Access out of range for std vector");
        } 
        return *(this->elems + pos);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type pos) {
        return *(this->elems + pos);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type pos) const {
        return *(this->elems + pos);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
        return *(this->elems);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
        return *(this->elems);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
        return *(this->elems + this->nelem - 1);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
        return *(this->elems + this->nelem - 1);
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::pointer vector<T, Allocator>::data() {
        return this->elems;
    }

    template<class T, class Allocator = std::allocator<T>>
    typename vector<T, Allocator>::const_pointer vector<T, Allocator>::data() const {
        return this->elems;
    }
}