#pragma once

#include "rb_tree_node.hpp"
#include <cstddef>
#include <iterator>

namespace mystd {
    template<class Key>
    rb_tree_iterator<Key>::rb_tree_iterator(const rb_tree_iterator& it) : ptr(v_it.ptr) {}

    template<class Key>
    rb_tree_iterator<Key>::rb_tree_iterator(base_node_type *ptr) : ptr(ptr) {}

    template<class Key>
    rb_tree_iterator<Key>::rb_tree_iterator() : ptr(nullptr) {}

    template<class Key>
    rb_tree_iterator<Key>& rb_tree_iterator<Key>::operator=(const rb_tree_iterator& rhs) {
        this->ptr = rhs.ptr;
    }

    template<class Key>
    rb_tree_iterator<Key>::operator rb_tree_iterator<const Key>() const {
        return rb_tree_node<const Key>(this->ptr);
    }

    template<class Key>
    rb_tree_iterator& rb_tree_iterator<Key>::operator++(void) {
        this->ptr = static_cast<node_type*>(this->ptr)->next();

        return *this;
    }

    template<class Key>
    rb_tree_iterator rb_tree_iterator<Key>::operator++(int) {
        rb_tree_iterator tmp = *this;
        this->ptr = static_cast<node_type*>(this->ptr)->next();

        return tmp;
    }

    template<class Key>
    rb_tree_iterator& rb_tree_iterator<Key>::operator--(void) {
        this->ptr = static_cast<node_type*>(this->ptr)->prev();

        return *this;
    }

    template<class Key>
    rb_tree_iterator rb_tree_iterator<Key>::operator--(int) {
        rb_tree_iterator tmp = *this;
        this->ptr = static_cast<node_type*>(this->ptr)->prev();

        return tmp;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator==(const rb_tree_iterator& rhs) const {
        return this->ptr == rhs.ptr;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator!=(const rb_tree_iterator& rhs) const {
        return this->ptr != rhs.ptr;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator<(const rb_tree_iterator& rhs) const {
        return this->ptr < rhs.ptr;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator>(const rb_tree_iterator& rhs) const {
        return this->ptr > rhs.ptr;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator<=(const rb_tree_iterator& rhs) const {
        return this->ptr <= rhs.ptr;
    }

    template<class Key>
    bool rb_tree_iterator<Key>::operator>=(const rb_tree_iterator& rhs) const {
        return this->ptr >= rhs.ptr;
    }

    template<class Key>
    typename rb_tree_iterator<Key>::reference rb_tree_iterator<Key>::operator*() const {
        return static_cast<node_type*>(this->ptr)->value;
    }

    template<class Key>
    typename rb_tree_iterator<Key>::pointer rb_tree_iterator<Key>::operator->() const {
        return addressof(this->operator*());
    }
}

#include "rb_tree_iterator.tpp"
