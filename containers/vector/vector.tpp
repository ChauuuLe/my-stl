#pragma once
#include "vector.hpp"

namespace mystd {
    /*Ctors*/
    template<class T, class Allocator = std::allocator<T>>
    vector<T, Allocator>::vector(): allocator(Allocator()), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(const Allocator& alloc): allocator(alloc), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(size_type count, const Allocator& alloc = Allocator()) {
        this->allocator = alloc;

        this->elems = std::allocator_traits<Allocator>::allocate(this->alloc, count);
        this->cap = count;

        try {

            this->nelem = count;
        } catch(...) {
            std::allocator_traits<Allocator>::allocate(this->alloc, this->elems, count);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(size_type count, const Allocator& alloc = Allocator()) {
        this->allocator = alloc;
        this->elems = std::allocator_traits<Allocator>::allocate(this->alloc, count);
        this->cap = count;

        try {
            std::uninitialized_value_construct_n(this->elems, count);
            this->nelem = count;
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->alloc, this->elems, count);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(size_type count, const T& value, const Allocator& alloc = Allocator()) {
        this->allocator = alloc;
        this->elems = std::allocator_traits<Allocator>::allocate(this->alloc, count);
        this->cap = count;

        try {
            for (size_type i = 0; i < size(); i++) {
                std::allocator_traits<Allocator>::construct(alloc, (this->elems + i), value);
            }
            this->nelem = count;
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->alloc, this->elems, count);
            throw;
        }
    }

    template<class T, class Allocator = std::allocator<T>>
    template<class InputIt>
    vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {
        this->allocator = alloc;
    }

    template<class T, class Allocator = std::allocator<T>>
    explicit vector<T, Allocator>::vector(const vector& other) {
        this->allocator = other.allocator;
        this->nelem = other.nelem;
        this->elems = std::allocator_traits<Allocator>::allocate(this->allocator, this->nelem);
        this->cap = this->nelem;

        try {
            for (size_type i = 0; i < this.size(); i++) {
                std::allocator_traits<Allocator>::construct(this->allocator, this->elems + i, *(other.elems + i));
            }
        } catch(...) {
            std::allocator_traits<Allocator>::deallocate(this->allocator, this->elems, this->nelem);
            throw;
        }
    }
}