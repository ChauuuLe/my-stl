#pragma once
#include "vector_iterator.hpp"

namespace mystd {
    /*Contructors and destructors*/
    template<class T>
    vector_iterator<T>::vector_iterator(): ptr(nullptr) {}

    template<class T>
    vector_iterator<T>::vector_iterator(const vector_iterator& rhs): ptr(rhs.ptr) {}

    template<class T>
    vector_iterator<T>::vector_iterator(value_type *ptr): ptr(ptr);

    template<class T>
    vector_iterator<T>& vector_iterator<T>::operator=(const vector_iterator& rhs) {
        this->ptr = rhs.ptr;

        return *this;
    }

    template<class T>
    vector_iterator<T>::operator vector<const T>() {
        return vector_iterator<const T>(this->ptr);
    }

    template<class T>
    vector_iterator<T>& vector_iterator<T>::operator++(void) {
        this->ptr++;

        return *this;
    }

    template<class T>
    vector_iterator<T> vector_iterator<T>::operator++(int) {
        vector_iterator<T> temp = vector_iterator<T>(*this);
        this->ptr++;

        return temp;
    }

    template<class T>
    vector_iterator<T>& vector_iterator<T>::operator--(void) {
        this->ptr--;

        return *this;
    }

    template<class T>
    vector_iterator<T> vector_iterator<T>::operator--(int) {
        vector_iterator<T> temp = vector_iterator<T>(*this);
        this->ptr--;

        return temp;
    }

    template<class T>
    bool vector_iterator<T>::operator==(const vector_iterator& rhs) const {
        return (this->ptr == rhs.ptr);
    }

    template<class T>
    bool vector_iterator<T>::operator!=(const vector_iterator& rhs) const {
        return (this->ptr != rhs.ptr);
    }

    template<class T>
    bool vector_iterator<T>::operator<(const vector_iterator& rhs) const {
        return (this->ptr < rhs.ptr);
    }

    template<class T>
    bool vector_iterator<T>::operator>(const vector_iterator& rhs) const {
        return (this->ptr > rhs.ptr);
    }

    template<class T>
    bool vector_iterator<T>::operator<=(const vector_iterator& rhs) const {
        return (this->ptr <= rhs.ptr);
    }

    template<class T>
    bool vector_iterator<T>::operator>=(const vector_iterator& rhs) const {
        return (this->ptr >= rhs.ptr);
    }

    template<class T>
    typename vector_iterator<T>::reference vector_iterator<T>::operator*() const {
        return *(this->ptr);
    }

    template<class T>
    typename vector_iterator<T>::reference vector_iterator<T>::operator[](typename vector_iterator<T>::difference_type n) const {
        return *(this->ptr + n);
    }

    template<class T>
    typename vector_iterator<T>::reference vector_iterator<T>::operator->() const {
        return this->ptr;
    }

    template<class T>
    vector_iterator<T> vector_iterator<T>::operator+(typename vector_iterator<T>::difference_type n) {
        vector_iterator<T> temp = vector_iterator<T>(*this);
        temp.ptr += n;

        return temp;
    }

    template<class T>
    vector_iterator<T> vector_iterator<T>::operator-(typename vector_iterator<T>::difference_type n) {
        vector_iterator<T> temp = vector_iterator<T>(*this);
        temp.ptr -= n;

        return temp;
    }

    template<class T>
    typename vector_iterator<T>::difference_type vector_iterator<T>::operator-(const vector_iterator<T>& rhs) const {
        return this->ptr - rhs.ptr;
    }

    template<class T>
    vector_iterator<T>& vector_iterator<T>::operator+=(typename vector_iterator<T>::difference_type n) {
        this->ptr += n;

        return *this;
    }

    template<class T>
    vector_iterator<T>& vector_iterator<T>::operator-=(typename vector_iterator<T>::difference_type n) {
        this->ptr -= n;

        return *this;
    }
}